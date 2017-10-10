int    mlock(const void *a, size_t b);
int    mlockall(int a);
void  *mmap(void *a, size_t b, int c, int d, int e, off_t f);
int    mprotect(void *a, size_t b, int c);
int    msync(void *a, size_t b, int c);
int    munlock(const void *a, size_t b);
int    munlockall(void);
int    munmap(void *a, size_t b);
int    posix_madvise(void *a, size_t b, int c);
int    posix_mem_offset(const void *restrict a, size_t b, off_t *restrict c, size_t *restrict d, int *restrict e);
//unsupported (missing type on Linux): int    posix_typed_mem_get_info(int a, struct posix_typed_mem_info *b);
int    posix_typed_mem_open(const char *a, int b, int c);
int    shm_open(const char *a, int b, mode_t c);
int    shm_unlink(const char *a);