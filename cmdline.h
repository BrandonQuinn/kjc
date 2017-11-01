/*
 * cmdline.h
 *
 *  Created on: 31 Oct. 2017
 *      Author: Brandon
 */

#ifndef CMDLINE_H
#define CMDLINE_H

#include "cmdline.h"
#include <iostream>
#include <map>
#include <vector>

#define MAX_ARG_LEN 1024

class a_CmdLine
{
	public:
		/*
		 * Adds an option to the command line.
		 * Returns true if the option already exists.
		 */
		bool AddOption(const char*);

		/*
		 * Add an argument to the option.
		 * Returns true if the option does not exist.
		 */
		bool AddArg(const char*, const char*);

		/*
		 * Simple linear search.
		 * Returns true if the option exists.
		 */
		bool HasOption(const char*);

		/**
		 * Returns all the arguments in the order they were entered
		 * for the given option.
		 */
		std::vector<std::string> GetArgs(const char*);
	private:
		std::map<std::string, std::vector<std::string> > opArgs;
};

#endif /* !CMDLINE_H */

