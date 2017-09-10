// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>

//#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char **argv){

	//argv[0] = storyboardCombine                                                                                        
	//argv[1] = target combined file                                                                                     
	//argv[2...n] = files to be combined                                                                                 

	FILE *mainFile;
	mainFile = fopen(argv[1], "a");

	char line[1024];
	int fileNo = 2, eventFlag = 0, i = 0;
	for (fileNo = 2; fileNo < argc; fileNo++) {
		FILE *partFile;
		partFile = fopen(argv[fileNo], "r");
		while (fgets(line, 1024, partFile)) {
			i++;
			if ((line[0] == '/' && line[1] == '/')) {
				continue;
			}
			else if ((!strcmp(line, "[Events]\n") && eventFlag)) {
				continue;
			}
			fprintf(mainFile, "%s", line);
			eventFlag = 1;
		}
		fclose(partFile);
	}
	fprintf(mainFile, "//");

	fclose(mainFile);

	return 0;
}
