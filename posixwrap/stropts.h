int    fattach(int a, const char *b);
int    fdetach(const char *a);
int    getmsg(int a, struct strbuf *restrict b, struct strbuf *restrict c, int *restrict d);
int    getpmsg(int a, struct strbuf *restrict b, struct strbuf *restrict c, int *restrict d, int *restrict e);
//unsupported (variable args): int    ioctl(int a, int b, ...);
int    isastream(int a);
int    putmsg(int a, const struct strbuf *b, const struct strbuf *c, int d);
int    putpmsg(int a, const struct strbuf *b, const struct strbuf *c, int d, int e);
