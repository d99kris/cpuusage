int  pselect(int a, fd_set *restrict b, fd_set *restrict c, fd_set *restrict d, const struct timespec *restrict e, const sigset_t *restrict f);
int  select(int a, fd_set *restrict b, fd_set *restrict c, fd_set *restrict d, struct timeval *restrict e);
