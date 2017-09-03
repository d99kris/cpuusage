//unsupported (variable args): int   semctl(int a, int b, int c, ...);
int   semget(key_t a, int b, int c);
int   semop(int a, struct sembuf *b, size_t c);
