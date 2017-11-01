/*
 * compiler.cpp
 *
 *  Created on: 31 Oct. 2017
 *      Author: Brandon
 */

#include "cmdline.h"
#include "compiler.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <dirent.h>
#include <queue>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>

using namespace std;

bool compiler::useLibraries = true;
std::vector<std::string> compiler::libFiles;
std::vector<std::string> compiler::srcFiles;

/*=========================================================
 * SOURCE FILES
 =========================================================*/

bool compiler::IsSourceFile(
		char* fileStr
) {
	char newStr[MAX_STR_LEN]; strcpy(newStr, fileStr);
	char* tok = strtok(newStr, ".");
	while(tok != NULL) {
		// kotlin or java file?
		if (strcmp(tok, JAVA_EXT) == 0
			|| strcmp(tok, KOTLIN_EXT) == 0) {
			return true;
		}
		tok = strtok(NULL, ".");
	}
	return false;
}

void compiler::RecursiveSearchSource(
		const char* src,
		vector<string>* dirs,
		queue<string>* queue
) {
	string current = queue->front();
	queue->pop();
	DIR* dir;
	dirent* ent;

	if ((dir = opendir (current.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			// get info to check if file or directory
			char newStr[MAX_STR_LEN] = "";
			strcat(newStr, current.c_str());
			strcat(newStr, "\\");
			strcat(newStr, ent->d_name);

			// ignore current and parent directory and make
			// sure it's a directory
			struct stat statPath;
			stat(newStr, &statPath);
			if (strcmp(ent->d_name, ".") != 0
					&& strcmp(ent->d_name, "..") != 0
					&& ent->d_name[0] != '.' // no hidden dirs
					&& S_ISDIR(statPath.st_mode)) {
				queue->push(string(newStr));
				// add the directory to dirs
				string result = string(newStr).substr(string(src).length() + 1,
						string(newStr).length());
				dirs->push_back(result.c_str());
				// recurse
				RecursiveSearchSource(src, dirs, queue);
			} else if (S_ISREG(statPath.st_mode) && IsSourceFile(newStr)) { // file
				// get the filename relative to the src path
				string result = string(newStr).substr(string(src).length() + 1,
						string(newStr).length());
				// add to list
				compiler::srcFiles.push_back(result.c_str());
			}
		}
		closedir (dir);
	} else {
		return;
	}
}

void compiler::GetSourceFiles (
		const char* src,
		vector<string>* dirs
) {
	vector<string>* ds = dirs;
	queue<string>* dirQueue = new queue<string>;
	dirQueue->push(string(src));
	RecursiveSearchSource(src, ds, dirQueue);
	delete dirQueue;
}

bool compiler::IsKotlinFile (
		const char* file
) {
	char newStr[MAX_STR_LEN]; strcpy(newStr, file);
	char* tok = strtok(newStr, ".");
	while(tok != NULL) {
		// kotlin or java file?
		if (strcmp(tok, KOTLIN_EXT) == 0) return true;
		tok = strtok(NULL, ".");
	}
	return false;
}

bool compiler::IsJavaFile (
		const char* file
) {
	char newStr[MAX_STR_LEN]; strcpy(newStr, file);
	char* tok = strtok(newStr, ".");
	while(tok != NULL) {
		// kotlin or java file?
		if (strcmp(tok, JAVA_EXT) == 0) return true;
		tok = strtok(NULL, ".");
	}
	return false;
}

void compiler::CopyDirs (
		vector<string> directories,
		const char* src,
		const char* dest
) {
	// create all the directories in the desination
	printf("Making destination directories...\n");
	for (unsigned int i = 0; i < directories.size(); i++) {
		char srcDir[MAX_STR_LEN] = "";
		char destDir[MAX_STR_LEN] = "";
		strcat(srcDir, src);
		strcat(srcDir, "\\");
		strcat(srcDir, directories.at(i).c_str());
		strcat(destDir, dest);
		strcat(destDir, "\\");
		strcat(destDir, directories.at(i).c_str());

		char cmd[266] = "mkdir ";
		strcat(cmd, destDir);
		system(cmd);

		// copy the files in the directory
		strcpy(cmd, "copy ");
		strcat(cmd, srcDir);
		strcat(cmd, "\\*.* ");
		strcat(cmd, destDir);
		system(cmd);
	}
	printf("Directories done.\n");
}

/*=========================================================
 * LIBRARY FILES
 =========================================================*/

bool compiler::IsLibraryFile(
		char* fileStr
) {
	char newStr[MAX_STR_LEN]; strcpy(newStr, fileStr);
	char* tok = strtok(newStr, ".");
	while(tok != NULL) {
		// kotlin or java file?
		if (strcmp(tok, JAVALIB_EXT) == 0
			|| strcmp(tok, WIN_DYN_LIB) == 0
			|| strcmp(tok, NIX_DYN_LIB) == 0
			|| strcmp(tok, MAC_DYN_LIB) == 0) {
			return true;
		}
		tok = strtok(NULL, ".");
	}
	return false;
}

void compiler::CopyLibraryFiles (
		const char* src,
		const char* dest
) {
	// create all the directories in the desination
	for (unsigned int i = 0; i < libFiles.size(); i++) {
		char cmd[MAX_STR_LEN] = "";

		// copy the files in the directory
		strcpy(cmd, "copy ");
		strcat(cmd, libFiles.at(i).c_str());
		strcat(cmd, " ");
		strcat(cmd, dest);
		system(cmd);
	}
}

void compiler::RecursiveLibrarySearch (
		const char* src,
		queue<string>* queue
) {
	string current = queue->front();
	queue->pop();
	DIR* dir;
	dirent* ent;

	if ((dir = opendir (current.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			// get info to check if file or directory
			char newStr[MAX_STR_LEN] = "";
			strcat(newStr, current.c_str());
			strcat(newStr, "\\");
			strcat(newStr, ent->d_name);

			// ignore current and parent directory and make
			// sure it's a directory
			struct stat statPath;
			stat(newStr, &statPath);
			if (strcmp(ent->d_name, ".") != 0
					&& strcmp(ent->d_name, "..") != 0
					&& ent->d_name[0] != '.' // no hidden dirs
					&& S_ISDIR(statPath.st_mode)) {
				queue->push(string(newStr));
				// recurse
				RecursiveLibrarySearch(src, queue);
			} else if (S_ISREG(statPath.st_mode) && IsLibraryFile(newStr)) { // file
				// get the filename relative to the src path
				string result = string(newStr);
				// add to list
				libFiles.push_back(result.c_str());
			}
		}
		closedir (dir);
	} else {
		return;
	}
}

void compiler::GetLibraryFiles (
		const char* src
) {
	queue<string>* dirQueue = new queue<string>;
	dirQueue->push(string(src));
	RecursiveLibrarySearch(src, dirQueue);
	delete dirQueue;
}

void compiler::CopyLibraries (
		const char* src,
		const char* dest
) {
	useLibraries = true;
	system((string("mkdir ") + string(dest)).c_str());
	GetLibraryFiles(src);
	CopyLibraryFiles(src, dest);
}

/*=========================================================
 * COMPILE FUNCTIONS
 =========================================================*/

void compiler::CompileSourceToClassFiles (
		const char* src,
		const char* dest
) {
	clock_t startTime = clock();
	vector<string> srcFiles;
	vector<string> directories;

	// java compiler invocation command
	string javaCommand = "";
	javaCommand += string("cd ") + string(dest)
			+ string(" & ") + string(JAVAC) + string(" -cp \".;*\" ");

	// kotlin compiler invocation command
	string kotlinCommand = "";
	kotlinCommand += string("cd ") + string(dest) + string(" & ") + string(KOTLINC) ;

	// Kotlin doesn't like wildcards... yet
	if (compiler::useLibraries) {
		kotlinCommand += string(" -cp \".;");
		for (unsigned int i = 0; i < compiler::libFiles.size(); i++) {
			kotlinCommand += string(compiler::libFiles.at(i)) + ";";
		}
		kotlinCommand += "\" ";
	}

	printf("Searching for Java and Kotlin source files...\n");
	GetSourceFiles(src, &directories);

	bool hasKotlin = false;
	bool hasJava = false;
	for (unsigned int i = 0; i < compiler::srcFiles.size(); i++) {
		if (IsKotlinFile(compiler::srcFiles.at(i).c_str())) {
			hasKotlin = true;
			kotlinCommand += compiler::srcFiles.at(i) + " ";
		} else if (IsJavaFile(compiler::srcFiles.at(i).c_str())) {
			hasJava = true;
			javaCommand += compiler::srcFiles.at(i) + " ";
		}
	}

	// copy files from src to dest
	CopyDirs(directories, src, dest);

	if (hasJava) {
		printf("Compilng Java source...\n");
		system(javaCommand.c_str());
		printf("Java compilation done.\n");
	} else {
		printf("No Java source found.\n");
	}

	if (hasKotlin) {
		printf("Compilng Kotlin source...\n");
		system(kotlinCommand.c_str());
		printf("Kotlin compilation done.\n");
	} else {
		printf("No Kotlin source found.\n");
	}

	printf("Source search done.\n");

	if (!hasJava && !hasKotlin) {
		printf("ERROR: No source files found of either type.\n");
		exit(EXIT_FAILURE);
	}

	startTime = clock() - startTime;
	printf("\nDone.\nCompilation time: %fs\n",
			((float)startTime) / CLOCKS_PER_SEC);
}

void compiler::CompileSourceToJar(
		const char* src,
		const char* dest,
		const char* mainClass
) {

}
