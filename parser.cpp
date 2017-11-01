#include <iostream>
#include <cstring>
#include <cstdlib>
#include "parser.h"

a_CmdLine a_Parse(
		const int argc,
		char** args
) {
	a_CmdLine cmdline;
	char curOp[MAX_ARG_LEN];
	strcpy(curOp, "");
	for (int i = 1; i < argc; i++) {
		// option
		if (args[i][0] == '-') {
			strcpy(curOp, strtok(args[i], "-"));
			// add option
			bool exists = cmdline.AddOption(curOp);
			// option already exist, error
			if (exists) {
				printf("Duplicate option '%s' found, ", curOp);
				printf("please only use it once.\n");
				exit (EXIT_FAILURE);
			}
			// argument
		} else {
			if (strcmp(curOp, "") != 0) {
				// add argument
				bool noOp = cmdline.AddArg(curOp, args[i]);
				// no option found, display error, exit
				if (noOp) {
					printf("You have not added option '%s' ", curOp);
					printf("to add agument '%s' to.\n", args[i]);
					exit (EXIT_FAILURE);
				}
				// no option to add arguments to
			} else {
				printf("First argument must be an option.\n");
				exit (EXIT_FAILURE);
			}
		}
	}
	return cmdline;
}
