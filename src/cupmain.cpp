/*
 * cupmain.cpp
 *
 * Copyright (C) 2020 Kristofer Berggren
 * All rights reserved.
 * 
 * cpuusage is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

/* ----------- Includes ------------------------------------------ */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>


/* ----------- File Global Variables ----------------------------- */
static int64_t isCollapsed = 1;
static int64_t isLogArgs = 0;
static int64_t isFirstProcess = 0;
static char default_path[32];
static char* path = NULL;
static int64_t beginTime = 0;
static int64_t endTime = 0;


/* ----------- Local Functions ----------------------------------- */
static void cup_writeheader()
{
  // open report file
  FILE *outfile = fopen(path, "w");
  if (outfile == NULL)
  {
    fprintf(stderr, "cpuusage: unable to write to output path '%s'\n", path);
    return;
  }

  // lock file
  if (flock(fileno(outfile), LOCK_EX) != 0)
  {
    fprintf(stderr, "cpuusage: unable to lock output path '%s'\n", path);
    return;
  }

  // header timestamp
  time_t global_time;
  struct tm* local_tm;
  char timestr[26] = { 0 };
  time(&global_time);
  local_tm = localtime(&global_time);
  strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S %z", local_tm);

  // header os
  char osstr[256] = { 0 };
  struct utsname uts;
  if (uname(&uts) == 0)
  {
    snprintf(osstr, sizeof(osstr), "%s %s", uts.sysname, uts.release);
  }

  // report header
  fprintf(outfile, "{\n");
  fprintf(outfile, "\"otherData\": {\n");
  fprintf(outfile, "  \"timestamp\":\"%s\",\n", timestr);
  fprintf(outfile, "  \"os\":\"%s\",\n", osstr);
  fprintf(outfile, "  \"cores\":\"%d\"\n", std::thread::hardware_concurrency());
  fprintf(outfile, "},\n");

  // report samples
  fprintf(outfile, "\"traceEvents\": [\n  ");

  // unlock file
  fflush(outfile);
  flock(fileno(outfile), LOCK_UN);
  
  // close file
  fclose(outfile);  
}

static std::string cup_escapejson(const std::string& p_Str)
{
  std::ostringstream oss;
  for (auto ch = p_Str.cbegin(); ch != p_Str.cend(); ch++)
  {
    if ((*ch == '"') || (*ch == '\\') || (('\x00' >= *ch) && (*ch <= '\x1f')))
    {
      oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)*ch;
    }
    else
    {
      oss << *ch;
    }
  }

  return oss.str();
}

static std::string cup_getprocid()
{
  pid_t pid = getpid();
  std::string pidstr = std::to_string(pid);
  return pidstr;
}

static std::string cup_getproccmd()
{
  pid_t pid = getpid();
  std::string pidstr = std::to_string(pid);
  std::string pidbracketstr = "[" + pidstr + "]";
  char tempname[] = "/tmp/cpuusage.XX" "XX" "XX";
  int tmpfd = mkstemp(tempname);
  if (tmpfd == -1)
  {
    return pidbracketstr;
  }

  close(tmpfd);

  // spawn child process
  pid_t childpid = fork();
  if (childpid == -1)
  {
    // failed to fork()
    return pidbracketstr;
  } 
  else if (childpid > 0)
  {
    // parent
    int status = 0;
    waitpid(childpid, &status, 0);
  }
  else 
  {
    // child
    int outfd = open(tempname, O_WRONLY | O_CREAT);
    if (outfd == -1)
    {
      fprintf(stderr, "cpuusage: open %s error\n", tempname);
      _exit(EXIT_FAILURE);
    }
    
    if (dup2(outfd, STDOUT_FILENO) == -1)
    {
      fprintf(stderr, "cpuusage: dup2(%d, %d) error\n", outfd, STDOUT_FILENO);
      _exit(EXIT_FAILURE);
    }

    // run ps
    execle("/bin/ps", "ps", "-p", pidstr.c_str(), "-o", "args", NULL, NULL);
    _exit(EXIT_FAILURE); // unreachable
  }

  // get second line from ps output
  std::string cmd;
  std::string line;
  std::ifstream infile(tempname);
  if (std::getline(infile, line) && std::getline(infile, line))
  {
    // escape control chars for json
    line = cup_escapejson(line);
    cmd = line;
  }

  // clean up
  infile.close();
  remove(tempname);
  
  return cmd;
}

static void cup_writeevent()
{
  // get process id/cmd
  std::string id = cup_getprocid();
  std::string cmd = cup_getproccmd();

  // open report file
  FILE *outfile = fopen(path, "a");
  if (outfile == NULL)
  {
    fprintf(stderr, "cpuusage: unable to append to output path '%s'\n", path);
    return;
  }

  // lock file
  if (flock(fileno(outfile), LOCK_EX) != 0)
  {
    fprintf(stderr, "cpuusage: unable to lock output path '%s'\n", path);
    return;
  }

  // write entry
  const pid_t pid = 0;
  const pid_t tid = isCollapsed ? 0 : getpid();
  const int64_t ts = beginTime;
  const int64_t dur = endTime - beginTime;
  fprintf(outfile, "{ \"ph\":\"X\", \"cat\":\"perf\", \"pid\":%d, "
          "\"tid\":%d, \"ts\":%" PRId64 ", \"dur\":%" PRId64 ", \"name\":\"%s\"",
          pid, tid, ts, dur, cmd.c_str());
  if (isLogArgs)
  {
    fprintf(outfile, ", \"args\": { \"cmd\":\"%s\", \"pid\":%d }", cmd.c_str(), getpid());
  }
  
  fprintf(outfile, " }");

  // special handling for first process / last event
  if (isFirstProcess == 1)
  {
    fprintf(outfile, "  ");
  }
  else
  {
    fprintf(outfile, ",\n  ");
  }
  
  // write report footer if it's the main process
  if (isFirstProcess == 1)
  {
    fprintf(outfile, "\n");
    fprintf(outfile, "]\n");
    fprintf(outfile, "}\n");
  }

  // unlock file
  fflush(outfile);
  flock(fileno(outfile), LOCK_UN);

  // close file
  fclose(outfile);
}


/* ----------- Global Functions ---------------------------------- */
void __attribute__ ((constructor)) cup_init(void)
{
  if (getenv("LD_PRELOAD") == NULL) return;

  path = getenv("CU_FILE");

  // determine report path
  if (path == NULL)
  {
    snprintf(default_path, sizeof(default_path), "./culog-%d.json", getpid());
    path = default_path;
  }

  // check if file does not exist
  if (access(path, F_OK) == -1)
  {
    // remember this is the main process
    isFirstProcess = 1;

    // write header
    cup_writeheader();
  }

  // store begin timestamp
  struct timeval tv;
  gettimeofday(&tv, NULL);
  beginTime = ((int64_t)tv.tv_sec * 1000000ll) + ((int64_t)tv.tv_usec);
}

void __attribute__ ((destructor)) cup_fini(void)
{
  if (getenv("LD_PRELOAD") == NULL) return;

  // store end timestamp
  struct timeval tv;
  gettimeofday(&tv, NULL);
  endTime = ((int64_t)tv.tv_sec * 1000000ll) + ((int64_t)tv.tv_usec);

  // write event to file
  cup_writeevent();
}
