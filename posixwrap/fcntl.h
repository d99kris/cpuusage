int  creat(const char *a, mode_t b);
//unsupported (variable args): int  fcntl(int, int, ...);
//unsupported (variable args): int  open(const char *, int, ...);
//unsupported (variable args): int  openat(int, const char *, int, ...);
int  posix_fadvise(int a, off_t b, off_t c, int d);
int  posix_fallocate(int a, off_t b, off_t c);
