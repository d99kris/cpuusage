void *shmat(int a, const void *b, int c);
int   shmctl(int a, int b, struct shmid_ds *c);
int   shmdt(const void *a);
int   shmget(key_t a, size_t b, int c);
