int       iswblank(wint_t a);
wint_t    iswascii(wint_t a);
wint_t    iswhexnumber(wint_t a);
wint_t    iswideogram(wint_t a);
wint_t    iswnumber(wint_t a);
wint_t    iswphonogram(wint_t a);
wint_t    iswrune(wint_t a);
wint_t    iswspecial(wint_t a);
wint_t    nextwctype(wint_t a, wctype_t b);
wint_t    towctrans(wint_t a, wctrans_t b);
wctrans_t wctrans(const char *c);