#ifndef LIB_CORE_CORE_HPP
#define LIB_CORE_CORE_HPP

#include <cstdio>

class Core {
public:
	Core();

	Core(
		char *inputFilePath,
		char *outputFilePath,
		char *insertionTagIdentifier
		);

	Core(
		char *inputFilePath,
		char *outputFilePath,
		char *commentOpen,
		char *insertionTagID,
		char tagSeparator,
		char *commentClose
		);

	~Core();

	int openInputFile();
	int openOutputFile();
	int openInsertionFile();

	char *inputFilePath;
	char *outputFilePath;
	char *insertionFilePath;

	FILE *inputFile;
	FILE *outputFile;
	FILE *insertionFile;
	long inputFileSize;

	char *commentOpen;
	char *insertionTagID;
	char tagSeparator;
	char *commentClose;

private:
};

#endif
