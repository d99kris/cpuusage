void           endgrent(void);
struct group  *getgrent(void);
struct group  *getgrgid(gid_t a);
int            getgrgid_r(gid_t a, struct group *b, char *c, size_t d, struct group **e);
struct group  *getgrnam(const char *a);
int            getgrnam_r(const char *a, struct group *b, char *c, size_t d, struct group **e);
void           setgrent(void);
