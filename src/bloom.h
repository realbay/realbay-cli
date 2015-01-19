
#ifndef REALBAY_BLOOM_H
#define REALBAY_BLOOM_H

#include <stdint.h>
#include "sha1.h"

typedef struct bloom_t {
	uint32_t *words;
	uint32_t seed;
	size_t hashCount;
	size_t bitCount;
	size_t byteCount;
	size_t wordCount;
} bloom_t;

void       bloom_init(bloom_t *bloom, uint32_t *words, const size_t bitCount, const size_t hashCount);
void       bloom_clear(bloom_t *bloom);
void       bloom_add(bloom_t *bloom, const uint8_t *hash);
int        bloom_check(bloom_t *bloom, const uint8_t *hash);
void       bloom_set_bit(bloom_t *bloom, const uint32_t absoluteBit);
uint32_t   bloom_hash_to_bit(const uint8_t *hash);

#endif // REALBAY_BLOOM_H

