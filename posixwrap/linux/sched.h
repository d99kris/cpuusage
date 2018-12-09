int    sched_get_priority_max(int a);
int    sched_get_priority_min(int a);
int    sched_getparam(pid_t a, struct sched_param *b);
int    sched_getscheduler(pid_t a);
int    sched_rr_get_interval(pid_t a, struct timespec *b);
int    sched_setparam(pid_t a, const struct sched_param *b);
int    sched_setscheduler(pid_t a, int b, const struct sched_param *c);
int    sched_yield(void);
