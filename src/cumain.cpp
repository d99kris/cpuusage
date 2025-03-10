/*
 * cumain.cpp
 *
 * Copyright (C) 2017-2025 Kristofer Berggren
 * All rights reserved.
 *
 * cpuusage is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

/* ----------- Includes ------------------------------------------ */
#include <atomic>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <cxxabi.h>
#include <dlfcn.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <unistd.h>


/* ----------- Types --------------------------------------------- */
struct Sample
{
  pthread_t thread;
  int64_t ts;
  void* func;
  bool enter;
};


/* ----------- File Global Variables ----------------------------- */
static std::map<void*, std::string>* symbols = NULL;
static std::vector<Sample>* samples = NULL;
static std::atomic_flag samples_lock = ATOMIC_FLAG_INIT;
static std::atomic<bool> tracing_enabled(false);
static int64_t count_samples = 0;
static int64_t max_samples = 1000000;
static int64_t min_time = 0;
static bool main_thread_only = false;
static pthread_t main_thread;


/* ----------- Local Function Prototypes ------------------------- */
static void cu_start_tracing(int);
static void cu_stop_tracing(int);
static void cu_trace_report();
static inline void cu_log_event(void *func, bool enter);
static std::string cu_get_timestamp_str();
static std::string cu_get_os_str();
static int cu_get_cores();
static std::string cu_addr_to_symbol(void *addr);


/* ----------- Global Functions ---------------------------------- */
void __attribute__ ((constructor)) cu_init(void)
{
  if (getenv("LD_PRELOAD") == NULL) return;

  main_thread = pthread_self();

  char *manual = getenv("CU_MANUAL");
  if ((manual != NULL) && (strncmp(manual, "1", 1) == 0))
  {
    signal(SIGUSR1, cu_start_tracing);
    signal(SIGUSR2, cu_stop_tracing);
  }
  else
  {
    cu_start_tracing(0);
  }
}

void __attribute__ ((destructor)) cu_fini(void)
{
  if (getenv("LD_PRELOAD") == NULL) return;

  cu_stop_tracing(0);
}

extern "C"
{
void __cyg_profile_func_enter(void *func, void * /*caller*/)
{
  cu_log_event(func, true);
}

void __cyg_profile_func_exit(void *func, void * /*caller*/)
{
  cu_log_event(func, false);
}

void cu_start_tracing()
{
  cu_start_tracing(0);
}

void cu_stop_tracing()
{
  cu_stop_tracing(0);
}

void cu_start_event_sym(void* sym_addr)
{
  cu_log_event(sym_addr, true);
}

void cu_end_event_sym(void* sym_addr)
{
  cu_log_event(sym_addr, false);
}

void cu_start_event(void)
{
  cu_log_event(__builtin_return_address(0), true);
}

void cu_end_event(void)
{
  cu_log_event(__builtin_return_address(0), false);
}
}


/* ----------- Local Functions ----------------------------------- */
static void cu_start_tracing(int)
{
  char *max_samples_str = getenv("CU_MAX_SAMPLES");
  if (max_samples_str != NULL)
  {
    max_samples = strtoll(max_samples_str, NULL, 10);
  }

  min_time = 0;
  char *min_time_str = getenv("CU_MIN_TIME");
  if (min_time_str != NULL)
  {
    min_time = strtoll(min_time_str, NULL, 10);
  }

  char *main_thread_only_str = getenv("CU_MAIN_THREAD_ONLY");
  main_thread_only = (main_thread_only_str != NULL) && (strncmp(main_thread_only_str, "1", 1) == 0);

  if (samples == NULL)
  {
    samples = new std::vector<Sample>;
    samples->reserve(max_samples);
  }

  if (symbols == NULL)
  {
    symbols = new std::map<void*, std::string>;
  }

  tracing_enabled = true;
}

static void cu_stop_tracing(int)
{
  tracing_enabled = false;

  if ((samples != NULL) && (symbols != NULL))
  {
    cu_trace_report();
    delete samples;
    delete symbols;
    samples = NULL;
    symbols = NULL;
    count_samples = 0;
  }
}

static inline void cu_log_event(void *func, bool enter)
{
  if (!tracing_enabled) return;

  if (main_thread_only && !pthread_equal(pthread_self(), main_thread)) return;

  // set function pointer, thread and type (enter / exit)
  Sample sample;
  sample.enter = enter;
  sample.func = func;
  sample.thread = pthread_self();

  // set current time stamp
  struct timeval tv;
  gettimeofday(&tv, NULL);
  sample.ts = ((int64_t)tv.tv_sec * 1000000ll) + ((int64_t)tv.tv_usec);

  // acquire spin lock
  while (samples_lock.test_and_set(std::memory_order_acquire))
  {
  }

  // for min_time (currently only supported for single thread trace), calculate duration from previous sample
  if (main_thread_only && (min_time > 0) && !enter && !samples->empty() && ((sample.ts - samples->back().ts) < min_time) &&
      samples->back().enter && (samples->back().func == sample.func))
  {
    // skip sample, drop last enter and update sample count
    samples->pop_back();
    --count_samples;
  }
  else
  {
    // store sample, update sample count, and check upper limit
    samples->push_back(sample);
    ++count_samples;
    if (count_samples >= max_samples) tracing_enabled = false;
  }

  // release spin lock
  samples_lock.clear(std::memory_order_release);
}

static void cu_trace_report()
{
  char default_path[32];
  char *path = getenv("CU_FILE");

  // determine report path
  if (path == NULL)
  {
    snprintf(default_path, sizeof(default_path), "./culog-%d.json", getpid());
    path = default_path;
  }

  // open report file
  FILE *outfile = fopen(path, "w");
  if (outfile == NULL)
  {
    fprintf(stderr, "cpuusage: unable to write to output path '%s'\n", path);
    return;
  }

  // report header
  fprintf(outfile, "{\n");
  fprintf(outfile, "\"otherData\": {\n");
  fprintf(outfile, "  \"timestamp\":\"%s\",\n", cu_get_timestamp_str().c_str());
  fprintf(outfile, "  \"os\":\"%s\",\n", cu_get_os_str().c_str());
  fprintf(outfile, "  \"cores\":\"%d\"\n", cu_get_cores());
  fprintf(outfile, "},\n");

  // report samples
  fprintf(outfile, "\"traceEvents\": [\n");
  bool first_sample = true;
  pid_t pid = getpid();
  for (auto const& sample : *samples)
  {
    if (first_sample)
    {
      first_sample = false;
      fprintf(outfile, "  ");
    }
    else
    {
      fprintf(outfile, ",\n  ");
    }
    char type = sample.enter ? 'B' : 'E';
    std::string name = cu_addr_to_symbol(sample.func);

    fprintf(outfile, "{ \"ph\":\"%c\", \"cat\":\"perf\", \"pid\":\"%d\", "
            "\"tid\":\"%p\", \"name\":\"%s\", \"ts\":%" PRId64 " }",
            type, pid, (void*)sample.thread, name.c_str(), sample.ts);
  }

  fprintf(outfile, "\n");
  fprintf(outfile, "]\n");
  fprintf(outfile, "}\n");
  fclose(outfile);
}

static std::string cu_get_timestamp_str()
{
  time_t global_time;
  struct tm* local_tm;
  char timestr[26];

  time(&global_time);
  local_tm = localtime(&global_time);
  strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S %z", local_tm);

  return std::string(timestr);
}

static std::string cu_get_os_str()
{
  struct utsname uts;

  if (uname(&uts) == 0)
  {
    return std::string(uts.sysname) + std::string(" ") + std::string(uts.release);
  }

  return std::string();
}

static int cu_get_cores()
{
  return std::thread::hardware_concurrency();
}

static std::string cu_addr_to_symbol(void *addr)
{
  std::string symbol;

  auto it = symbols->find(addr);
  if (it != symbols->end())
  {
    symbol = it->second;
  }
  else
  {
    Dl_info dlinfo;
    if (dladdr(addr, &dlinfo) && dlinfo.dli_sname)
    {
      if (dlinfo.dli_sname[0] == '_')
      {
        int status = -1;
        char *demangled = NULL;
        demangled = abi::__cxa_demangle(dlinfo.dli_sname, NULL, 0, &status);
        if (demangled)
        {
          if (status == 0)
          {
              symbol = std::string(demangled);
          }
          free(demangled);
        }
      }

      if (symbol.empty())
      {
        symbol = std::string(dlinfo.dli_sname);
      }

      if (!symbol.empty())
      {
        symbol += std::string(" + ");
        symbol += std::string(std::to_string((char*)addr - (char*)dlinfo.dli_saddr));
      }
    }
    else
    {
      std::stringstream ss;
      ss << "0x" << std::hex << (unsigned long long)addr;
      symbol = ss.str();
    }

    (*symbols)[addr] = symbol;
  }

  return symbol;
}

