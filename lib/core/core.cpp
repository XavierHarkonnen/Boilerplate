#include "core.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

Core::Core()
	: inputFilePath(nullptr),
		outputFilePath(nullptr),
		commentOpen(strdup("<!--")),
		insertionTagID(strdup("BOILERPLATE-INSERT")),
		tagSeparator(':'),
		commentClose(strdup("-->")) {
}

Core::Core(char *inputFilePath, char *outputFilePath, char *insertionTagID)
	: inputFilePath(inputFilePath),
		outputFilePath(outputFilePath),
		commentOpen(strdup("<!--")),
		insertionTagID(insertionTagID),
		tagSeparator(':'),
		commentClose(strdup("-->")) {
}

Core::Core(
	char *inputFilePath,
	char *outputFilePath,
	char *commentOpen,
	char *insertionTagID,
	char tagSeparator,
	char *commentClose
	)
	: inputFilePath(inputFilePath),
		outputFilePath(outputFilePath),
		commentOpen(commentOpen),
		insertionTagID(insertionTagID),
		tagSeparator(tagSeparator),
		commentClose(commentClose) {
}

Core::~Core() {
	free(inputFilePath);
	free(outputFilePath);
	free(commentOpen);
	free(insertionTagID);
	free(commentClose);

	if (inputFile) {
		fclose(inputFile);
		inputFile = nullptr;
	}

	if (outputFile) {
		fclose(outputFile);
		outputFile = nullptr;
	}

	if (insertionFile) {
		fclose(insertionFile);
		insertionFile = nullptr;
	}
}

int Core::openInputFile() {
	inputFile = fopen(inputFilePath, "r");

	if (!inputFile) { return -1; }

	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	return 0;
}

int Core::openOutputFile() {
	outputFile = fopen(outputFilePath, "w");
	if (!outputFile) { return -1; }
	return 0;
}

int Core::openInsertionFile() {
	insertionFile = fopen(insertionFilePath, "r");
	if (!insertionFile) { return -1; }
	return 0;
}