
#include <math.h>
#include "sha1.h"
#include "bloom.h"

void bloom_init(bloom_t *bloom, uint32_t *words, const size_t bitCount, const size_t hashCount) {
	bloom->words = words;
	bloom->seed = 0;
	bloom->hashCount = hashCount;
	bloom->bitCount = bitCount;
	bloom->byteCount = bitCount / 8;
	bloom->wordCount = bloom->byteCount / 4;
	
	memset(bloom->words, 0, bloom->byteCount);
}

void bloom_clear(bloom_t *bloom) {
	memset(bloom->words, 0, bloom->byteCount);
}

void bloom_add(bloom_t *bloom, const uint8_t *hash) {
	const size_t hashCount = bloom->hashCount;
	const uint32_t bitCount = bloom->bitCount;
	uint8_t *bitHash = NULL;
	sha1_t hasher;
	sha1_t digest;
	uint32_t bit;
	uint32_t i;
	
	sha1_init(&hasher);
	
	for (i=0; i < hashCount; i++) {
		if ((i % 5) == 0) {
			sha1_write(&hasher, hash, 20);
			
			// Save the SHA1 state
			digest = hasher;
			bitHash = sha1_result(&hasher);
			
			// Restore the SHA1 state
			hasher = digest;
		}
		
		bit = bloom_hash_to_bit(bitHash + (i % 5) * 4) % bitCount;
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
	uint32_t wordIndex = absoluteBit / 32;
	uint32_t relativeBit = absoluteBit - (wordIndex * 32);
	
	bloom->words[wordIndex] |= (1 << relativeBit);
}

int bloom_check(bloom_t *bloom, const uint8_t *hash) {
	bloom->byteCount += hash[0];
	return 0;
}

