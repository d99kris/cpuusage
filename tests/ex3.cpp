/*
 * ex3.cpp
 *
 * Copyright (C) 2018 Kristofer Berggren
 * All rights reserved.
 * 
 * cpuusage is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

/* ----------- Includes ------------------------------------------ */
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cpuusage.h"


/* ----------- Function Prototypes ------------------------------- */
void copy_large_file();
void copy_medium_file();
void copy_small_file();
void copy_tiny_file();


/* ----------- Global Functions ---------------------------------- */
int main(void)
{
  setenv("CU_MAX_SAMPLES", "1000", 1);
  setenv("CU_FILE", "culog.json", 1);
    
  cu_start_tracing();

  copy_large_file();
  copy_medium_file();
  copy_small_file();
  copy_tiny_file();

  cu_stop_tracing();

  return 0;
}

void copy_large_file()
{
  cu_start_event();

  int large_size = 1024 * 1024 * 1024;
  char* buf = (char*) calloc(1, large_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, large_size, 1, fin);
  fclose(fin);
  fwrite(buf, large_size, 1, fout);
  fclose(fout);
  free(buf);

  cu_end_event();
}

void copy_medium_file()
{
  cu_start_event_sym((void*) &copy_medium_file);

  int medium_size = 128 * 1024 * 1024;
  char* buf = (char*) calloc(1, medium_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, medium_size, 1, fin);
  fclose(fin);
  fwrite(buf, medium_size, 1, fout);
  fclose(fout);
  free(buf);

  cu_end_event_sym((void*) &copy_medium_file);
}

void copy_small_file()
{
  cu_scoped_event scoped_event;

  int small_size = 1024 * 1024;
  char* buf = (char*) calloc(1, small_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, small_size, 1, fin);
  fclose(fin);
  fwrite(buf, small_size, 1, fout);
  fclose(fout);
  free(buf);
}

void copy_tiny_file()
{
  cu_scoped_event scoped_event((void*) &copy_tiny_file);

  int tiny_size = 256 * 1024;
  char* buf = (char*)calloc(1, tiny_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, tiny_size, 1, fin);
  fclose(fin);
  fwrite(buf, tiny_size, 1, fout);
  fclose(fout);
  free(buf);
}
