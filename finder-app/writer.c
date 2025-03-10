
#include "stdint.h"
#include "stdio.h"

#include <sys/stat.h>

int main(int argc, char** argv)
{
	struct stat stats;

	if (argc != 3) 
	{
		printf("Usage: write <path> <print string>\n");
		return 1;
	}

	const char* filePath = argv[1];
	const char* printString = argv[2];

	FILE* mFile = fopen(filePath, "wt"); //no append
	if (mFile)
	{
		fprintf(mFile, "%s\n", printString);
		fclose(mFile);
	}

	return 1;
}
