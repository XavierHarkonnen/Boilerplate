#include "command_line_options.hpp"

#include "metadata.h"

Option options[] = {
	Option('h', "help", false, help),
	Option('v', "version", false, version),
	Option('\0', "info", false, info),
	Option('i', "input", true, inputFilePath),
	Option('o', "output", true, outputFilePath),
	Option('\0', "opening", true, commentOpen),
	Option('t', "tag", true, insertionTagID),
	Option('\0', "separator", true, tagSeparator),
	Option('\0', "closing", true, commentClose)
};

size_t optionCount = sizeof(options) / sizeof(options[0]);

int help(const char */*argument*/, Core */*core*/) {
	printf("%s %i.%i.%i\n", PROJECT_NAME, PROJECT_MAJOR_VERSION, PROJECT_MINOR_VERSION, PROJECT_PATCH_VERSION);
	puts("Usage: descent [options]");
	puts("Options:");
	puts("  -h, --help         Show this help message and exit");
	puts("  -v, --version      Show version information and exit");
	puts("      --info         Show detailed information about the engine and exit");
	puts("  -i, --input        Set the input file");
	puts("  -o, --output       Set the output file");
	puts("  -t, --tag          Set the marker for boilerplate insertion tags (default \"BOILERPLATE-INSERT\")");
	puts("      --separator    Set the separator for boilerplate insertion tags (default \':\')");
	puts("      --opening      Set the opening marker for comments (default \"<!--\")");
	puts("      --closing      Set the closing marker for comments (default \"-->\")");

	exit(0);
}

int version(const char */*argument*/, Core */*core*/) {
	printf("%s %i.%i.%i\n", PROJECT_NAME, PROJECT_MAJOR_VERSION, PROJECT_MINOR_VERSION, PROJECT_PATCH_VERSION);
	printf("Released %s\n", PROJECT_RELEASE_DATE);
	printf("Copyright (C) 2024 %s\n", PROJECT_AUTHOR);
	printf("Licensed under %s\n", PROJECT_LICENSE);
	printf("%s\n", PROJECT_LICENSE_SUMMARY);

	exit(0);
}

int info(const char */*argument*/, Core */*core*/) {
	printf("%s %i.%i.%i (%s)\n", PROJECT_NAME, PROJECT_MAJOR_VERSION, PROJECT_MINOR_VERSION, PROJECT_PATCH_VERSION, PROJECT_STATUS);
	printf("Released %s\n", PROJECT_RELEASE_DATE);
	printf("Copyright (C) 2024 %s\n", PROJECT_AUTHOR);
	printf("Licensed under %s\n", PROJECT_LICENSE);
	printf("%s\n", PROJECT_LICENSE_TEXT);
	printf("\n");
	printf("%s\n", PROJECT_DESCRIPTION);
	printf("Organization Homepage: %s\n", PROJECT_ORGANIZATION_HOMEPAGE);
	printf("Project Homepage: %s\n", PROJECT_PROJECT_HOMEPAGE);
	printf("\n");
	printf("Compiled on %s\n", PROJECT_BUILD_DATE);
	printf("Built for %s using %s\n", PROJECT_PLATFORM, PROJECT_COMPILER);
	printf("\n");
	printf("Contributors: %s\n", PROJECT_CONTRIBUTORS);

	exit(0);
}

int inputFilePath(const char *argument, Core *core) {
	core->inputFilePath = strdup(argument);
	if (!(core->inputFilePath)) { return -1; }
	return 0;
}

int outputFilePath(const char *argument, Core *core) {
	core->outputFilePath = strdup(argument);
	if (!(core->outputFilePath)) { return -1; }
	return 0;
}

int commentOpen(const char *argument, Core *core) {
	core->commentOpen = strdup(argument);
	if (!(core->commentOpen)) { return -1; }
	return 0;
}

int insertionTagID(const char *argument, Core *core) {
	core->insertionTagID = strdup(argument);
	if (!(core->insertionTagID)) { return -1; }
	return 0;
}

int tagSeparator(const char *argument, Core *core) {
	if (argument[0] == '\0') { return -1; }
	core->tagSeparator = argument[0];
	if (argument[1] != '\0') { return 1; }
	return 0;
}

int commentClose(const char *argument, Core *core) {
	core->commentClose = strdup(argument);
	if (!(core->commentClose)) { return -1; }
	return 0;
}
