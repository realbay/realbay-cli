
#ifndef REALBAY_HEX
#define REALBAY_HEX

#include <stdint.h>
#include <stddef.h>

void      hex_decode(const char *str, uint8_t *bytes);
void      hex_encode(const uint8_t *bytes, char *str);
uint8_t   hex_decode_char(const char c);
char      hex_encode_nibble(const uint8_t nibble);
void      hex_encode_byte(const uint8_t byte, char *a, char *b);

#endif // REALBAY_HEX
