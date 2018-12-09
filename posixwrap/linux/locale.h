locale_t      duplocale(locale_t a);
void          freelocale(locale_t a);
struct lconv *localeconv(void);
locale_t      newlocale(int a, const char *b, locale_t c);
char         *setlocale(int a, const char *b);
locale_t      uselocale(locale_t a);
