uint32_t htonl(uint32_t a);
uint16_t htons(uint16_t a);
uint32_t ntohl(uint32_t a);
uint16_t ntohs(uint16_t a);
in_addr_t    inet_addr(const char *a);
char        *inet_ntoa(struct in_addr a);
const char  *inet_ntop(int a, const void *restrict b, char *restrict c, socklen_t d);
int          inet_pton(int a, const char *restrict b, void *restrict c);
