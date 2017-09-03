size_t  iconv(iconv_t a, char **restrict b, size_t *restrict c, char **restrict d, size_t *restrict e);
int     iconv_close(iconv_t a);
iconv_t iconv_open(const char *a, const char *b);
