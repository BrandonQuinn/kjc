/*
 * cmdline.cpp
 *
 *  Created on: 31 Oct. 2017
 *      Author: Brandon
 */

#include <cstring>
#include "cmdline.h"

using namespace std;

bool a_CmdLine::AddOption(
		const char* op
) {
	// option alreadt exists
	if (HasOption(op)) return true;
	opArgs[op] = vector<string>();
	return false;
}

bool a_CmdLine::AddArg(
		const char* op,
		const char* arg
) {
	// option doesn't exist
	if (opArgs.count(op) == 0) return true;
	opArgs[op].push_back(arg);
	return false;
}

bool a_CmdLine::HasOption(
		const char* op
) {
	if (opArgs.count(op) == 1) return true;
	return false;
}

vector<string> a_CmdLine::GetArgs(
		const char* op
) {
	return opArgs[op];
}
