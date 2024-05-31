#ifndef COMMAND_LINE_OPTIONS_HPP
#define COMMAND_LINE_OPTIONS_HPP

#include <stdlib.h>
#include <ctype.h>

#include "../lib/cl_input/parse_arguments.hpp"

extern Option options[];
extern size_t optionCount;

int help(const char */*argument*/, Core */*core*/);
int version(const char */*argument*/, Core */*core*/);
int info(const char */*argument*/, Core */*core*/);

int inputFilePath(const char *argument, Core *core);
int outputFilePath(const char *argument, Core *core);
int commentOpen(const char *argument, Core *core);
int insertionTagID(const char *argument, Core *core);
int tagSeparator(const char *argument, Core *core);
int commentClose(const char *argument, Core *core);

#endif