void          endutxent(void);
struct utmpx *getutxent(void);
struct utmpx *getutxid(const struct utmpx *a);
struct utmpx *getutxline(const struct utmpx *a);
struct utmpx *pututxline(const struct utmpx *a);
void          setutxent(void);
