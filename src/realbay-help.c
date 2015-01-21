
#include "realbay.h"

#include <string.h>
#include <stdio.h>

realbay_main_func *help_funcs[] = {
	realbay_help_usage,
	realbay_help_createindex,
	realbay_help_hashes,
	realbay_help_findrecords,
	NULL
};

const char *help_names[] = {
	"help",
	"createindex",
	"hashes",
	"findrecords",
	NULL
};

const int help_command_count = 4;

declare_main(realbay_help_main) {
	realbay_main_func *f;
	const char *command;
	const char *str;
	int i;
	
	if (argc <= 2) {
		command = "help";
	} else {
		command = argv[2];
	}
	
	fprintf(stderr,
		" \n"
		" RealBay Command Line 0.2.1 \n"
		" \n"
	);
	
	for (i=0; i < help_command_count; i++) {
		str = help_names[i];
		f = help_funcs[i];
		
		if (str == NULL || f == NULL) {
			break;
		}
		
		if (strcmp(str, command) == 0) {
			return f(argc, argv);
		}
	}
	
	printf("ERROR: Cannot find help for unknown command '%s'\n", command);
	return realbay_help_usage(argc, argv);
}

declare_main(realbay_help_usage) {
	puts(
		" USAGE: realbay <command> [...] \n"
		" \n"
		" COMMANDS \n"
		"    help           Read help text on a specific command \n"
		"    createindex    Create a new index from a CSV file \n"
		"    verify         Verify which pieces of an index are valid \n"
		"    findpieces     Search for pieces by keyword \n"
		"    findrecords    Find all records in valid pieces by keyword \n"
		"    hashes         Outputs all the hashes in an index \n"
		" \n"
		" For specific information on a command use: \n"
		" \n"
		"    realbay help verify  # Gives you help the verify command \n"
		" \n"
		" Please file bugs at https://github.com/realbay/realbay \n"
	);
	
	return 0;
}


