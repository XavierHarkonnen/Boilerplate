#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../lib/cl_input/parse_arguments.hpp"
#include "../lib/core/core.hpp"

#include "command_line_options.hpp"

#define COMMENT_BUFFER_SIZE 1024

enum ScanState {
	DEFAULT,
	NEW_LINE,
	COMMENT_OPENING,
	COMMENT_CONTENT,
	COMMENT_CLOSING
};

void validate(int result, Core *core) {
		if (result) {
		puts("Command line argument parsing failed");
		exit(-1);
	} if (!core->inputFilePath) {
		puts("Input file path is NULL");
		exit(-1);
	} if (!core->outputFilePath) {
		puts("Output file path is NULL");
		exit(-1);
	} if (!core->commentOpen) {
		puts("Comment opening marker is NULL");
		exit(-1);
	} if (!core->insertionTagID) {
		puts("Boilerplate insertion tag marker is NULL");
		exit(-1);
	} if (!core->commentClose) {
		puts("Comment closing marker is NULL");
		exit(-1);
	} if (core->openInputFile()) {
		printf("Could not open input file. Is \"%s\" a valid file path?\n", core->inputFilePath);
		exit(-1);
	} if (core->openOutputFile()) {
		printf("Could not open output file. Is \"%s\" a valid file path?\n", core->outputFilePath);
		exit(-1);
	}
}

void flushOutputBuffer(FILE *outputFile, char *outputBuffer, long *outputBufferHead) {
	fwrite(outputBuffer, 1, *outputBufferHead, outputFile);
	*outputBufferHead = 0;
}

void consumeWhitespace(char *buffer, unsigned int *head) {
	while ((buffer[*head] != '\0') && ((buffer[*head] == ' ') || (buffer[*head] == '\t'))) {
		++(*head);
	}
}

void parseCommentBuffer(Core *core, char *commentBuffer, char *outputBuffer, long *outputBufferHead) {
	unsigned int commentBufferHead = 0;
	unsigned int tagIDIndex = 0;

	consumeWhitespace(commentBuffer, &commentBufferHead);

	bool tagMatch = false;

	while (commentBuffer[commentBufferHead++] == core->insertionTagID[tagIDIndex++]) {
		if (core->insertionTagID[tagIDIndex] == '\0') {
			tagMatch = true;
			break;
		}
	}

	if (!tagMatch) {
		return;
	}

	consumeWhitespace(commentBuffer, &commentBufferHead);

	if (commentBuffer[commentBufferHead] != core->tagSeparator) {
		return;
	}

	++commentBufferHead;

	consumeWhitespace(commentBuffer, &commentBufferHead);

	core->insertionFilePath = (commentBuffer + commentBufferHead);
	if(core->openInsertionFile()) {
		printf("Could not open insertion file. Is \"%s\" a valid file path?\n", core->insertionFilePath);
		exit(-1);
	}

	flushOutputBuffer(core->outputFile, outputBuffer, outputBufferHead);

	size_t bytesRead;
	while ((bytesRead = fread(outputBuffer, 1, core->inputFileSize, core->insertionFile)) > 0) {
		size_t bytesWritten = fwrite(outputBuffer, 1, bytesRead, core->outputFile);
		if (bytesWritten != bytesRead) {
			puts("Error writing to file");
			fclose(core->insertionFile);
			core->insertionFile = nullptr;
			fclose(core->outputFile);
			core->outputFile = nullptr;
			exit(-1);
		}
	}

	fclose(core->insertionFile);
	core->insertionFile = nullptr;
	core->insertionFilePath = nullptr;

	outputBuffer[(*outputBufferHead)++] = '\n';
}

void handleCommentBufferOverflow() {
	puts("Comment buffer overflow.");
	exit(-1);
}

void processInputBuffer(Core *core, char *inputBuffer, char *outputBuffer) {
	char whitespaceBuffer[COMMENT_BUFFER_SIZE];
	char commentBuffer[COMMENT_BUFFER_SIZE];
	char auxiliaryBuffer[COMMENT_BUFFER_SIZE];

	ScanState scanState = DEFAULT;

	long outputBufferHead = 0;
	unsigned int whitespaceBufferHead = 0;
	unsigned int commentBufferHead = 0;
	unsigned int auxiliaryBufferHead = 0;
	int commentOpenIndex = 0;
	int commentCloseIndex = 0;

	for (long inputBufferHead = 0; inputBufferHead < core->inputFileSize; ++inputBufferHead) {

		if (commentBufferHead > COMMENT_BUFFER_SIZE) {
			handleCommentBufferOverflow();
		}

		if (outputBufferHead > core->inputFileSize) {
			flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
		}

		switch (scanState) {
			case DEFAULT:
				if (inputBuffer[inputBufferHead] == '\n') {
					scanState = NEW_LINE;
					outputBuffer[outputBufferHead++] = inputBuffer[inputBufferHead];
				} else if (inputBuffer[inputBufferHead] == core->commentOpen[commentOpenIndex++]) {
					if (core->commentOpen[commentOpenIndex] == '\0') {
						scanState = COMMENT_CONTENT;
						commentOpenIndex = 0;
					} else {
						scanState = COMMENT_OPENING;
						commentBuffer[commentBufferHead++] = inputBuffer[inputBufferHead];
					}
				} else {
					for (unsigned int i = 0; i < whitespaceBufferHead; ++i) {
						outputBuffer[outputBufferHead++] = whitespaceBuffer[i];
						if (outputBufferHead > core->inputFileSize) {
							flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
						}
					}
					whitespaceBufferHead = 0;
					outputBuffer[outputBufferHead++] = inputBuffer[inputBufferHead];
					commentOpenIndex = 0;
				}
				break;

			case NEW_LINE:
				while ((inputBuffer[inputBufferHead] == ' ') || (inputBuffer[inputBufferHead] == '\t')) {
					whitespaceBuffer[whitespaceBufferHead++] = inputBuffer[inputBufferHead++];
				}
				if (inputBuffer[inputBufferHead] == core->commentOpen[commentOpenIndex++]) {
					if (core->commentOpen[commentOpenIndex] == '\0') {
						scanState = COMMENT_CONTENT;
						commentOpenIndex = 0;
					} else {
						scanState = COMMENT_OPENING;
						commentBuffer[commentBufferHead++] = inputBuffer[inputBufferHead];
					}
				} else {
					for (unsigned int i = 0; i < whitespaceBufferHead; ++i) {
						outputBuffer[outputBufferHead++] = whitespaceBuffer[i];
						if (outputBufferHead > core->inputFileSize) {
							flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
						}
					}
					whitespaceBufferHead = 0;
					outputBuffer[outputBufferHead++] = inputBuffer[inputBufferHead];
					commentOpenIndex = 0;
				}
				break;

			case COMMENT_OPENING:
				if (inputBuffer[inputBufferHead] == core->commentOpen[commentOpenIndex++]) {
					if (core->commentOpen[commentOpenIndex] == '\0') {
						scanState = COMMENT_CONTENT;
						commentOpenIndex = 0;
						commentBufferHead = 0;
					} else {
						commentBuffer[commentBufferHead++] = inputBuffer[inputBufferHead];
					}
				} else {
					scanState = DEFAULT;
					commentOpenIndex = 0;
					
					for (unsigned int i = 0; i < whitespaceBufferHead; ++i) {
						outputBuffer[outputBufferHead++] = whitespaceBuffer[i];
						if (outputBufferHead > core->inputFileSize) {
							flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
						}
					}
					whitespaceBufferHead = 0;

					for (unsigned int i = 0; i < commentBufferHead; ++i) {
						outputBuffer[outputBufferHead++] = commentBuffer[i];
						if (outputBufferHead > core->inputFileSize) {
							flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
						}
					}
					commentBufferHead = 0;
					outputBuffer[outputBufferHead++] = inputBuffer[inputBufferHead];
				}
				break;

			case COMMENT_CONTENT:
				if (inputBuffer[inputBufferHead] == core->commentClose[commentCloseIndex++]) {
					if (core->commentClose[commentCloseIndex] == '\0') {
						scanState = DEFAULT;
						commentCloseIndex = 0;
						commentBuffer[commentBufferHead] = '\0';

						if (((inputBufferHead + 1) < core->inputFileSize) && (inputBuffer[inputBufferHead + 1] == '\n')) {
							++inputBufferHead;
							whitespaceBufferHead = 0;
						} else {
							for (unsigned int i = 0; i < whitespaceBufferHead; ++i) {
								outputBuffer[outputBufferHead++] = whitespaceBuffer[i];
								if (outputBufferHead > core->inputFileSize) {
									flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
								}
							}
							whitespaceBufferHead = 0;
						}

						parseCommentBuffer(core, commentBuffer, outputBuffer, &outputBufferHead);
						commentBufferHead = 0;
					} else {
						scanState = COMMENT_CLOSING;
						auxiliaryBuffer[auxiliaryBufferHead++] = inputBuffer[inputBufferHead];
					}
				} else {
					commentBuffer[commentBufferHead++] = inputBuffer[inputBufferHead];
					commentCloseIndex = 0;
				}
				break;

			case COMMENT_CLOSING:
				if (inputBuffer[inputBufferHead] == core->commentClose[commentCloseIndex++]) {
					if (core->commentClose[commentCloseIndex] == '\0') {
						scanState = DEFAULT;
						commentCloseIndex = 0;
						auxiliaryBufferHead = 0;
						commentBuffer[commentBufferHead] = '\0';

						if (((inputBufferHead + 1) < core->inputFileSize) && (inputBuffer[inputBufferHead + 1] == '\n')) {
							++inputBufferHead;
							whitespaceBufferHead = 0;
						} else {
							for (unsigned int i = 0; i < whitespaceBufferHead; ++i) {
								outputBuffer[outputBufferHead++] = whitespaceBuffer[i];
								if (outputBufferHead > core->inputFileSize) {
									flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
								}
							}
							whitespaceBufferHead = 0;
						}

						parseCommentBuffer(core, commentBuffer, outputBuffer, &outputBufferHead);
						commentBufferHead = 0;
					} else {
						auxiliaryBuffer[auxiliaryBufferHead++] = inputBuffer[inputBufferHead];
					}
				} else {
					scanState = COMMENT_CONTENT;
					commentCloseIndex = 0;
					for (unsigned int i = 0; i < auxiliaryBufferHead; ++i) {
						commentBuffer[commentBufferHead++] = auxiliaryBuffer[i];
					}
					if (commentBufferHead > COMMENT_BUFFER_SIZE) {
						handleCommentBufferOverflow();
					}
					auxiliaryBufferHead = 0;
					commentBuffer[commentBufferHead++] = inputBuffer[inputBufferHead];
				}
				break;
		}
	}
	if (outputBufferHead > 0) {
		flushOutputBuffer(core->outputFile, outputBuffer, &outputBufferHead);
	}
}

int main(int argc, char *argv[]) {
	Core core;

	validate(parseArguments(argc, argv, optionCount, options, &core), &core);

	char *inputBuffer = (char *)malloc(core.inputFileSize);
	if (!inputBuffer) { 
		puts("Failed to create input buffer");
		return -1;
	}
	fread(inputBuffer, 1, core.inputFileSize, core.inputFile);
	fclose(core.inputFile);
	core.inputFile = nullptr;


	char *outputBuffer = (char *)malloc(core.inputFileSize);
	if (!outputBuffer) { 
		puts("Failed to create output buffer");
		return -1;
	}

	memset(outputBuffer, 0, core.inputFileSize);

	processInputBuffer(&core, inputBuffer, outputBuffer);

	free(inputBuffer);
	free(outputBuffer);

	return 0;
}
