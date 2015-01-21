
#include <math.h>
#include "sha1.h"
#include "bloom.h"

#ifdef REALBAY_DEBUG
#include <stdio.h>
#endif

void bloom_init(bloom_t *bloom, uint32_t *words, const size_t bitCount, const size_t hashCount) {
	bloom->words = words;
	bloom->seed = 0;
	bloom->hashCount = hashCount;
	bloom->bitCount = bitCount;
	bloom->byteCount = bitCount / 8;
	bloom->wordCount = bloom->byteCount / 4;
	
	if (bloom->words) {
		memset(bloom->words, 0, bloom->byteCount);
	}
}

void bloom_clear(bloom_t *bloom) {
	memset(bloom->words, 0, bloom->byteCount);
}

#define bloom_eachbit(bloom, hash) \
	const size_t hashCount = bloom->hashCount; \
	const uint32_t bitCount = bloom->bitCount; \
	uint8_t *bitHash = NULL; \
	sha1_t hasher; \
	sha1_t digest; \
	uint32_t bit; \
	uint32_t i; \
	\
	sha1_init(&hasher); \
	\
	for (i=0; i < hashCount; i++) { \
		if ((i % 5) == 0) { \
			sha1_write(&hasher, hash, 20); \
			\
			digest = hasher; \
			bitHash = sha1_result(&hasher); \
			bit = bloom_hash_to_bit(bitHash) % bitCount; \
			hasher = digest; \
		} else { \
			bit = bloom_hash_to_bit(bitHash + (i % 5) * 4) % bitCount; \
		} 

int bloom_check(bloom_t *bloom, const uint8_t *hash) {
	bloom_eachbit(bloom, hash)
		if (!bloom_get_bit(bloom, bit)) {
			return 0;
		}
	}
	return 1;
}

int bloom_compare(const bloom_t *needle, const bloom_t *haystack) {
	const bloom_t *a = needle;
	const bloom_t *b = haystack;
	size_t i;
	uint32_t mask;
	int empty = 1;
	
	if (a->bitCount != b->bitCount) {
		#ifdef REALBAY_DEBUG
		fprintf(stderr, "Bit counts don't match (%d and %d)\n", a->bitCount, b->bitCount);
		#endif
		return 0;
	}
	
	for (i=0; i < a->wordCount; i++) {
		mask = a->words[i] & b->words[i];
		
		if (mask != a->words[i]) {
			return 0;
		}
		
		empty = empty && (a->words[i] == 0);
	}
	
	#ifdef REALBAY_DEBUG
	if (empty) {
		fprintf(stderr, "Bloom filter was empty\n");
	}
	#endif
	
	return !empty;
}

void bloom_add(bloom_t *bloom, const uint8_t *hash) {
	bloom_eachbit(bloom, hash)
		bloom_set_bit(bloom, bit);
	}
}

uint32_t bloom_hash_to_bit(const uint8_t *hash) {
	return (
		(hash[0]) |
		(hash[1] << 8) | 
		(hash[2] << 16) |
		(hash[3] << 24)
	);
}

void bloom_set_bit(bloom_t *bloom, const uint32_t absoluteBit) {
	const uint32_t wordIndex = absoluteBit / 32;
	const uint32_t relativeBit = absoluteBit - (wordIndex * 32);
	
	bloom->words[wordIndex] |= (1 << relativeBit);
}

int bloom_get_bit(bloom_t *bloom, const uint32_t absoluteBit) {
	const uint32_t wordIndex = absoluteBit / 32;
	const uint32_t relativeBit = absoluteBit - (wordIndex * 32);
	const uint32_t mask = (1 << relativeBit);
	
	return (bloom->words[wordIndex] & mask) == mask;
}

