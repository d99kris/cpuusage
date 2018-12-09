int    sem_close(sem_t *a);
//unsupported (variable args): sem_t *sem_open(const char *a, int b, ...);
int    sem_post(sem_t *a);
int    sem_trywait(sem_t *a);
int    sem_unlink(const char *a);
int    sem_wait(sem_t *a);
