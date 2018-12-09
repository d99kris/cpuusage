//unsupported (not common for platforms): int   getitimer(int a, struct itimerval *b);
//unsupported (internally used by cpuusage): int   gettimeofday(struct timeval *restrict a, void *restrict b);
//unsupported (not common for platforms): int   setitimer(int a, const struct itimerval *restrict b, struct itimerval *restrict c);
int   utimes(const char *a, const struct timeval b[2]);
