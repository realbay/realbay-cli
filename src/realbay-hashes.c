
#include "realbay.h"
#include "sha1.h"
#include "hex.h"
#include <stdio.h>

declare_main(realbay_help_hashes) {
	puts(
		" \n"
		" USAGE: realbay hashes <index> [OPTIONS] \n"
		" \n"
		" The 'hashes' command simply prints out every hash in the \n"
		" <index> file, which was created previously with the `createindex` \n"
		" command. Only hashes are output, since the index file does not \n"
		" include search term data. \n "
		" \n"
		" Every line of output is a hexidecimal hash, like this: \n"
		" \n"
		"     4e1243bd22c66e76c2ba9eddc1f91394e57f9f83 \n"
		"     b6f5c461b0f6cbc6425de47f4ca474472ffbcc53 \n"
		"     ... \n"
		"     <EOF> \n"
		" \n"
		" TODO This file needs to check the SHA1 of the lookup table to \n"
		"      it doesn't output junk data during partial downloads \n"
		" \n"
	);
	
	return 0;
}

declare_main(realbay_hashes_main) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: realbay hashes <index>\n");
		return 1;
	}
	
	if (!realbay_hashes(argv[2])) {
		fprintf(stderr, "ERROR: Failed to output hashes for index file\n");
		return 1;
	}
	
	return 0;
}

int realbay_hashes(const char *indexPath) {
	FILE *indexFile;
	uint8_t record[64];
	char hex[64];
	
	if (indexPath == NULL || indexPath[0] == '\0') {
		fprintf(stderr, "ERROR: Nothing passed to hashes\n");
		return 0;
	}
	
	indexFile = fopen(indexPath, "rb");
	
	if (!indexFile) {
		realbay_fopen_error(indexPath);
		return 0;
	}
	
	memset(record, 0, 64);
	
	while (!feof(indexFile)) {
		if (64 != fread(record, 1, 64, indexFile)) {
			realbay_io_error(indexFile, indexPath);
			return 0;
		}
		
		hex_encode(record + 44, hex);
		puts(hex);
	}
	
	fclose(indexFile);
	
	return 1;
}
