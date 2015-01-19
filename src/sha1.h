
#ifndef REALBAY_SHA1_H
#define REALBAY_SHA1_H

#include <ctype.h>
#include <stdint.h>
#include <string.h>


#ifdef __BIG_ENDIAN__
# define SHA_BIG_ENDIAN
#elif defined __LITTLE_ENDIAN__
/* override */
#elif defined __BYTE_ORDER
# if __BYTE_ORDER__ ==  __ORDER_BIG_ENDIAN__
# define SHA_BIG_ENDIAN
# endif
#else // ! defined __LITTLE_ENDIAN__
# include <endian.h> // machine/endian.h
# if __BYTE_ORDER__ ==  __ORDER_BIG_ENDIAN__
#  define SHA_BIG_ENDIAN
# endif
#endif


/* header */

#define HASH_LENGTH 20
#define BLOCK_LENGTH 64

typedef struct sha1_t {
	uint32_t buffer[BLOCK_LENGTH/4];
	uint32_t state[HASH_LENGTH/4];
	uint32_t byteCount;
	uint8_t bufferOffset;
	uint8_t keyBuffer[BLOCK_LENGTH];
	uint8_t innerHash[HASH_LENGTH];
} sha1_t;

typedef uint8_t sha1hash_t[HASH_LENGTH];

void sha1_init(sha1_t *s);
void sha1_writebyte(sha1_t *s, uint8_t data);
void sha1_write(sha1_t *s, const uint8_t *data, size_t len);
void sha1_write_str(sha1_t *s, const char *data, size_t len);
uint8_t* sha1_result(sha1_t *s);
void sha1_initHmac(sha1_t *s, const uint8_t* key, int keyLength);
uint8_t* sha1_resultHmac(sha1_t *s);

#endif // REALBAY_SHA1_H
