#include <iostream>
#include <cstdlib>
#include <fstream>
#include "cmdline.h"
#include "parser.h"
#include "compiler.h"

using namespace std;

int main(
		int argc,
		char* argv[]
) {
	a_CmdLine cmdline = a_Parse(argc, argv);

	bool src = cmdline.HasOption("s");
	bool dest = cmdline.HasOption("d");
	bool mc = cmdline.HasOption("mc");
	bool jar = cmdline.HasOption("jar");
	bool lib = cmdline.HasOption("lib");

	if (src && dest && !mc && !jar) { // compile to class files
		vector<string> srcArg = cmdline.GetArgs("s");
		vector<string> destArg = cmdline.GetArgs("d");

		printf("Compiling class files from: %s\n", srcArg[0].c_str());
		printf("Compiling class files to: %s\n", destArg[0].c_str());

		// copy library files to destination directory
		if (lib) {
			vector<string> libArg = cmdline.GetArgs("lib");
			printf("Copying library files from '%s'...\n", libArg[0].c_str());
			compiler::CopyLibraries(libArg[0].c_str(), destArg[0].c_str());
			printf("Library files copied.\n");
		}

		compiler::CompileSourceToClassFiles(
				srcArg[0].c_str(),
				destArg[0].c_str()
		);

	} else if (src && dest && mc && jar) { // compile to executable jar file
		vector<string> srcArg = cmdline.GetArgs("s");
		vector<string> destArg = cmdline.GetArgs("d");
		vector<string> mcArg = cmdline.GetArgs("mc");

		compiler::CompileSourceToJar(
				srcArg[0].c_str(),
				destArg[0].c_str(),
				mcArg[0].c_str()
		);
	} else {
		printf("Missing options... must have at ");
		printf("least -s (source directory) and ");
		printf("-d (destination directory).\n");
	}

	return EXIT_SUCCESS;
}
