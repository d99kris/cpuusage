int ftw(const char *a, int (*b)(const char *, const struct stat *, int), int c);
int nftw(const char *a, int (*b)(const char *, const struct stat *, int, struct FTW *), int c, int d);
