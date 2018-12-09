int      aio_cancel(int a, struct aiocb *b);
int      aio_error(const struct aiocb *a);
int      aio_fsync(int a, struct aiocb *b);
int      aio_read(struct aiocb *a);
ssize_t  aio_return(struct aiocb *b);
int      aio_suspend(const struct aiocb *const a[], int b, const struct timespec *c);
int      aio_write(struct aiocb *a);
