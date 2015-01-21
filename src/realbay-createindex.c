
#include <stdio.h>
#include <math.h>

#include "realbay.h"
#include "sha1.h"
#include "csv.h"
#include "bloom.h"
#include "keywords.h"
#include "hex.h"

#define MAX_LINE_LENGTH (1024)
#define MAX_PIECES (1024 * 8)

declare_main(realbay_help_createindex) {
	puts(
		" USAGE: realbay createindex <csv> <output> [OPTIONS] \n"
		" \n"
		" The 'createindex' command creates a new searchable index from \n"
		" an input CSV file. The CSV file is expected to have two columns, \n"
		" one which is a 'title' that will be scanned for words and the other \n"
		" is the infohash of the torrent. An example CSV: \n"
		" \n"
		"  'Ubuntu 14.10 64-bit Desktop', B415C913643 ... \n"
		"  'Ubuntu 14.04 LTS 64-bit Desktop', CB84CCC1... \n"
		" \n"
		" The size of the output file is based on the number of rows in the input \n"
		" CSV file. The total size of the index will be (n * 64) + (m * 512) where n \n"
		" is the number of torrents to be indexed and m is the number of pieces the \n"
		" index is divided into. \n"
		" \n"
	);
	
	return 0;
}

declare_main(realbay_createindex_main) {
	if (argc <= 3) {
		printf("ERROR: Not enough parameters\n");
		return 1;
	}
	
	#ifdef REALBAY_DEBUG
	printf("[DEBUG] Opening CSV file %s\n", argv[2]);
	printf("[DEBUG] Output file is %s\n", argv[3]);
	#endif
	
	realbay_createindex(argv[2], argv[3]);
	
	return 0;
}

int realbay_createindex(const char *csvPath, const char *outputPath) {
	FILE *csvFile = NULL;
	FILE *outFile = NULL;
	FILE *metaFile = NULL;
	csv_t csv;
	char metaPath[1024];
	char lineBuffer[MAX_LINE_LENGTH];
	unsigned int contentSize;
	unsigned int pieceSize;
	unsigned int contentPieceCount;
	unsigned int metaSize;
	unsigned int totalSize;
	unsigned int pieceCount;
	unsigned int recordsPerPiece;
	unsigned int recordCount = 0;
	unsigned int currentRecord = 0;
	uint8_t *hash;
	uint32_t pieceBloomBits[512];
	uint32_t recordBloomBits[11];
	bloom_t pieceBloom;
	bloom_t recordBloom;
	sha1_t hasher;
	uint8_t recordHash[20];
	
	csv_init(&csv, '|');
	
	sprintf(metaPath, "%s.lut", outputPath);
	DebugLog("Opening CSV file");
	csvFile = fopen(csvPath, "r");
	
	if (!csvFile) {
		realbay_fopen_error(csvPath);
		goto realbay_createindex_error;
	}
	
	outFile = fopen(outputPath, "wb");
	
	if (!outFile) {
		realbay_fopen_error(outputPath);
		goto realbay_createindex_error;
	}
	
	metaFile = fopen(metaPath, "wb");
	
	if (!metaFile) {
		realbay_fopen_error(metaPath);
		goto realbay_createindex_error;
	}
	
	memset(lineBuffer, 0, MAX_LINE_LENGTH);
	DebugLog("Counting records in CSV input...");
	
	while (fgets(lineBuffer, MAX_LINE_LENGTH, csvFile) != NULL) {
		recordCount++;
	}
	
	contentSize = recordCount * 64;
	pieceSize = 256 * 1024;
	contentPieceCount = (unsigned int)ceil((double)contentSize / (double)pieceSize);
	metaSize = 512 + (2048 * contentPieceCount);
	totalSize = metaSize + contentSize;
	pieceCount = (unsigned int)ceil((double)totalSize / (double)pieceSize);
	recordsPerPiece = pieceSize / 64;
	
	DebugLogf("[DEBUG] Counted %d lines in the CSV\n", recordCount);
	DebugLogf("[DEBUG] Content size is %d\n", contentSize);
	DebugLogf("[DEBUG] There are %d content pieces\n", contentPieceCount);
	DebugLogf("[DEBUG] Meta data size is %d\n", metaSize);
	DebugLogf("[DEBUG] Total data size is %d\n", totalSize);
	DebugLogf("[DEBUG] Total piece count is %d\n", pieceCount);
	
	currentRecord = 0;
	
	if (fseek(csvFile, 0, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: Unable to seek in CSV file '%s'\n", csvPath);
		goto realbay_createindex_error;
	}
	
	bloom_init(&pieceBloom, pieceBloomBits, 512 * 32, 12);
	bloom_init(&recordBloom, recordBloomBits, 44 * 8, 4);
	
	int row_handler(const csv_col_t *columns, const size_t columnCount) {
		//if ((currentRecord % 100000) == 0) {
		//	printf("%d / %d\n", currentRecord, recordCount);
		//}
		
		void keyword_handler(const char *keyword, const size_t keywordLen) {
			sha1_init(&hasher);
			sha1_write_str(&hasher, keyword, keywordLen);
			hash = sha1_result(&hasher);
			
			bloom_add(&recordBloom, hash);
			bloom_add(&pieceBloom, hash);
			
			//printf("%s\n", keyword);
		}
		
		bloom_clear(&recordBloom);
		keyword_parse(columns[0].text, keyword_handler);
		hex_decode(columns[2].text, recordHash, 20);
		
		#ifdef REALBAY_DEBUG
		char test[1024];
		hex_encode((uint8_t*)recordBloom.words, test, recordBloom.byteCount);
		fprintf(stderr, "B: 0x%s\n", test);
		#endif
		
		//printf("---\n");
		
		if (fwrite(recordBloomBits, 4, 11, outFile) != 11) {
			realbay_io_error(outFile, outputPath);
			return 1;
		}
		
		if (fwrite(recordHash, 1, 20, outFile) != 20) {
			realbay_io_error(outFile, outputPath);
			return 1;
		}
		
		currentRecord++;
		
		if ((currentRecord % recordsPerPiece) == 0) {
			fprintf(stderr, "piece %d\n", currentRecord / recordsPerPiece);
			
			if (fwrite(pieceBloomBits, 4, 512, metaFile) != 512) {
				realbay_io_error(metaFile, metaPath);
				return 1;
			}
			
			bloom_clear(&pieceBloom);
		}
		
		return 0;
	};
	
	csv_parse(&csv, csvFile, row_handler);
	
	if (!metaFile || !outFile) {
		goto realbay_createindex_error;
	}
	
	realbay_fclose(csvFile);
	realbay_fclose(outFile);
	realbay_fclose(metaFile);
	
	return 1;
	
realbay_createindex_error: ;
	
	realbay_fclose(csvFile);
	realbay_fclose(outFile);
	realbay_fclose(metaFile);
	
	return 0;
}
