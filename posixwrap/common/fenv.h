int  feclearexcept(int a);
int  fegetenv(fenv_t *a);
int  fegetexceptflag(fexcept_t *a, int b);
int  fegetround(void);
int  feholdexcept(fenv_t *a);
int  feraiseexcept(int a);
int  fesetenv(const fenv_t *a);
int  fesetexceptflag(const fexcept_t *a, int b);
int  fesetround(int a);
int  fetestexcept(int a);
int  feupdateenv(const fenv_t *a);