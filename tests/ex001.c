/*
 * ex001.c
 *
 * Copyright (C) 2017 Kristofer Berggren
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


/* ----------- Function Prototypes ------------------------------- */
void copy_large_file();
void copy_medium_file();
void copy_small_file();


/* ----------- Global Functions ---------------------------------- */
int main(void)
{
  copy_large_file();
  copy_medium_file();
  copy_small_file();
  return 0;
}

void copy_large_file()
{
  int large_size = 1024 * 1024 * 1024;
  char *buf = calloc(1, large_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, large_size, 1, fin);
  fclose(fin);
  fwrite(buf, large_size, 1, fout);
  fclose(fout);
  free(buf);
}

void copy_medium_file()
{
  int medium_size = 128 * 1024 * 1024;
  char *buf = calloc(1, medium_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, medium_size, 1, fin);
  fclose(fin);
  fwrite(buf, medium_size, 1, fout);
  fclose(fout);
  free(buf);
}

void copy_small_file()
{
  int small_size = 1024 * 1024;
  char *buf = calloc(1, small_size);
  FILE* fin = fopen("/dev/null", "r");
  FILE* fout = fopen("/dev/null", "w");
  fread(buf, small_size, 1, fin);
  fclose(fin);
  fwrite(buf, small_size, 1, fout);
  fclose(fout);
  free(buf);
}

