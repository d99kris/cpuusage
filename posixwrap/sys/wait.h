pid_t  wait(int *a);
int    waitid(idtype_t a, id_t b, siginfo_t *c, int d);
pid_t  waitpid(pid_t a, int *b, int c);
