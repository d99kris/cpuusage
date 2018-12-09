//unsupported (never returns): void   longjmp(jmp_buf a, int b);
//unsupported (never returns): void   siglongjmp(sigjmp_buf a, int b);
int    setjmp(jmp_buf a);
int    sigsetjmp(sigjmp_buf a, int b);
