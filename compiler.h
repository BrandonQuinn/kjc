/*
 * compiler.h
 *
 *  Created on: 31 Oct. 2017
 *      Author: Brandon
 */

#ifndef COMPILER_H
#define COMPILER_H

#include <queue>
#include <vector>

#define KOTLINC 		"kotlinc"
#define JAVAC 			"javac"
#define KOTLIN_EXT 		"kt"
#define JAVA_EXT 		"java"
#define JAVALIB_EXT 	"jar"
#define WIN_DYN_LIB 	"dll"
#define NIX_DYN_LIB		"so"
#define MAC_DYN_LIB		"dylib"

#define MAX_STR_LEN 1024

class compiler
{
	public:
		/*
		 * Good god no, this should probably be a class soon, everything is
		 * getting messy.
		 * Basically just says whether or not we need to at
		 * libraries to the kotlin class path.
		 */
		static bool useLibraries;
		static std::vector<std::string> libFiles;
		static std::vector<std::string> srcFiles;

		/*
		 * Compiles the source kotlin and java files into
		 * class files in the same package structure from the src
		 * directly to the destination.
		 */
		static void CompileSourceToClassFiles(
				const char* src,
				const char* dest
		);

		/*
		 * Compiles the source in the same manner as c_CompileSourceToClassFiles
		 * however, it also generates an executable jar file then deletes
		 * the left over class and source files, ONLY IN THE DESTINATION,
		 * THE SOURCE DIRECTORY IS UNTOUCHED!
		 */
		static void CompileSourceToJar(
				const char* src,
				const char* dest,
				const char* mainClass
		);

		/*
		 *Gets all the source files, Java or Kotlin from the given directory.
		 */
		static void GetSourceFiles(
				const char* src,
				std::vector<std::string>* dirs
		);

		/*
		 * Copy all files that are a type of library from the src directory
		 * to the destination directory.
		 * Ignores directory structure and copies all files found recursively
		 * in to the destination.
		 */
		static void CopyLibraries(
				const char* src,
				const char* dest
		);

		/*
		 * Is the given file path a library file?
		 */
		static bool IsLibraryFile(
				char* fileStr
		);

		/*
		 * Is the given file name a source file? Java or Kotlin?
		 */
		static bool IsSourceFile(
				char* fileStr
		);

		/*
		 * Is the given file a Kotlin source file?
		 */
		static bool IsKotlinFile(
				const char* file
		);

		/*
		 * Is the given file a Java source file?
		 */
		static bool IsJavaFile(
				const char* file
		);

	private:
		static void RecursiveSearchSource(
				const char* src,
				std::vector<std::string>* dirs,
				std::queue<std::string>* queue
		);

		static void RecursiveLibrarySearch(
				const char* src,
				std::queue<std::string>* queue
		);

		static void CopyDirs(
				std::vector<std::string> directories,
				const char* src,
				const char* dest
		);

		static void CopyLibraryFiles(
				const char* src,
				const char* dest
		);

		static void GetLibraryFiles(
				const char* src
		);
};

#endif /* !COMPILER_H */
