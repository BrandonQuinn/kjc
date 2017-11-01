/*
 * parser.h
 *
 *  Created on: 30 Oct. 2017
 *      Author: Brandon
 */

#ifndef PARSER_H
#define PARSER_H

#include <cstring>
#include "cmdline.h"

/*
 * Takes in the regular C/C++ arguments and checks
 * for options and the arguments to go with them
 * then adds these options and corresponding
 * arguments to a map in a_CmdLine.
 */
a_CmdLine a_Parse(
		const int argc,
		char** args
);

#endif /* !PARSER_H */
