int    regcomp(regex_t *restrict a, const char *restrict b, int c);
size_t regerror(int a, const regex_t *restrict b, char *restrict c, size_t d);
int    regexec(const regex_t *restrict a, const char *restrict b, size_t c, regmatch_t d[restrict], int e);
void   regfree(regex_t *a);
