
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "realbay.h"
#include "sha1.h"
#include "bloom.h"
#include "keywords.h"
#include "hex.h"

declare_main(realbay_help_findrecords) {
	puts(
		" USAGE: realbay findrecords <index> <keyword>... [OPTIONS] \n"
		" \n"
		" Using the `findrecords` command searches an <index> for records \n"
		" that match all of the given keywords. Each matching record found is \n"
		" printed out in hexidecimal as a 40-character hash (in lowercase) \n"
		" \n"
	);
	
	return 0;
}

declare_main(realbay_findrecords_main) {
	const char *indexPath = NULL;
	FILE *indexFile = NULL;
	size_t i;
	uint32_t searchBloomBits[REALBAY_RECORD_WORDS];
	bloom_t recordBloom;
	bloom_t searchBloom;
	sha1_t hasher;
	uint8_t *hashBytes = NULL;
	const char *keyword = NULL;
	uint8_t record[64];
	char hex[64];
	
	if (argc < 4) {
		fprintf(stderr, "USAGE: realbay findrecords <index> <keyword>...");
		return 1;
	}
	
	indexPath = argv[2];
	
	if (!indexPath || indexPath[0] == '\0') {
		fprintf(stderr, "ERROR: No path given for findrecords\n");
		return 1;
	}
	
	indexFile = fopen(indexPath, "rb");
	
	if (!indexFile) {
		fprintf(stderr, "ERROR: Cannot open file '%s'\n", indexPath);
		return 1;
	}
	
	DebugLog("Init bloom filters");
	bloom_init(&recordBloom, NULL, REALBAY_RECORD_BITS, REALBAY_RECORD_FUNCS);
	bloom_init(&searchBloom, searchBloomBits, REALBAY_RECORD_BITS, REALBAY_RECORD_FUNCS);
	
	DebugLog("Build search filer");
	
	for (i=3; i < (size_t)argc; i++) {
		keyword = argv[i];
		
		sha1_init(&hasher);
		sha1_write_str(&hasher, keyword, strlen(keyword));
		hashBytes = sha1_result(&hasher);
		
		bloom_add(&searchBloom, hashBytes);
	}
	
	fseek(indexFile, 0, SEEK_SET);
	
	while (!feof(indexFile)) {
		if (fread(record, 1, 64, indexFile) != 64) {
			break;
		}
		
		recordBloom.words = (uint32_t*)record;
		
		if (bloom_compare(&searchBloom, &recordBloom)) {
			hex_encode(record + 44, hex, 20);
			puts(hex);
		}
	}
	
	if (!indexFile) {
		fprintf(stderr, "ERROR: There was an error reading the index file\n");
		return 1;
	}
	
	realbay_fclose(indexFile);
	
	return 0;
}

