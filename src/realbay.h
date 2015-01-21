
#ifndef REALBAY_H
#define REALBAY_H

#include <stdint.h>

// Define this if you want debugging
#define REALBAY_DEBUG

// We want to log IO errors without clutter
#define realbay_fopen_error(path) \
	fprintf( \
		stderr, \
		"ERROR: Unable to open file %s\n", \
		path \
	); 
	
#define realbay_io_error(fp, path) \
	fprintf( \
		stderr, \
		"ERROR: Unable to read from file %s at offset %lu\n", \
		path, \
		ftell((FILE*)fp) \
	); \
	\
	if (fp != NULL) { \
		fclose((FILE*)fp); \
		fp = NULL; \
	} \

#define realbay_fclose(fp) if (fp) { fclose(fp); fp = NULL; }

// When not in debugging make the debugging functions do nothing
#ifdef REALBAY_DEBUG
#define DebugLog(msg) printf("[DEBUG] %s\n", msg)
#define DebugLogf(msg, args...) printf(msg, args)
#else
#define DebugLog(msg)
#define DebugLogf(msg, args...) 
#endif

#define REALBAY_RECORD_BYTES  (44)
#define REALBAY_RECORD_BITS   (44 * 8)
#define REALBAY_RECORD_WORDS  (44 / 4)
#define REALBAY_RECORD_FUNCS  (7)

#define REALBAY_PIECE_BYTES   (512)
#define REALBAY_PIECE_BITS    (512 * 8)
#define REALBAY_PIECE_WORDS   (512 / 4) 
#define REALBAY_PIECE_FUNCS   (13)

// Execution is redirected throughout the program to different
// main functions. This is the type definition of those functions
typedef int (realbay_main_func)(int, const char**);

typedef void (realbay_findrecords_func)(uint8_t *hash);

// Helper macro for main-like function signature
#define declare_main(name) int name(int argc, const char **argv)

declare_main(realbay_help_main);
declare_main(realbay_help_usage);
declare_main(realbay_help_createindex);
declare_main(realbay_help_hashes);
declare_main(realbay_help_findrecords);

declare_main(realbay_createindex_main);
declare_main(realbay_hashes_main);
declare_main(realbay_findrecords_main);


// Main API

int realbay_createindex(const char *csvPath, const char *outputPath);
int realbay_hashes(const char *indexPath);


#endif // REALBAY_H
