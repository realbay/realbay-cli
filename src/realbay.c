
#include <string.h>
#include <stdio.h>
#include "realbay.h"

const char *command_table[] = {
	"help",
	"createindex",
	"hashes",
	NULL
};

realbay_main_func *function_table[] = {
	realbay_help_main,
	realbay_createindex_main,
	realbay_hashes_main,
	NULL
};

const int command_count = 3;

declare_main(main) {
	int i = 0;
	const char *command = NULL;
	realbay_main_func *f = NULL;
	const char *str = NULL;
	
	if (argc <= 1) {
		command = "help";
	} else {
		command = argv[1];
	}
	
	for (i=0; i < command_count; i++) {
		str = command_table[i];
		f = function_table[i];
		
		if (str == NULL || f == NULL) {
			break;
		}
		
		if (strcmp(str, command) == 0) {
			return f(argc, argv);
		}
	}
	
	if (command == NULL) {
		fprintf(stderr, "ERROR: Unable to read command\n");
	} else {
		fprintf(stderr, "ERROR: Unknown command '%s'\n", command);
	}
	
	str = NULL;
	f = NULL;
	
	return 0;
}
