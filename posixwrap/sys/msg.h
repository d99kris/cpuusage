int       msgctl(int a, int b, struct msqid_ds *c);
int       msgget(key_t a, int b);
ssize_t   msgrcv(int a, void *b, size_t c, long d, int e);
int       msgsnd(int a, const void *b, size_t c, int d);
