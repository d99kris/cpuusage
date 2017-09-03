void           endpwent(void);
struct passwd *getpwent(void);
struct passwd *getpwnam(const char *a);
int            getpwnam_r(const char *a, struct passwd *b, char *c, size_t d, struct passwd **e);
struct passwd *getpwuid(uid_t a);
int            getpwuid_r(uid_t a, struct passwd *b, char *c, size_t d, struct passwd **e);
void           setpwent(void);
