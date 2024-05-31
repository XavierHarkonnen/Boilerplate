#include "parse_arguments.hpp"

#include <string.h>

Option::Option(char shortName, const char *longName, bool takesArgument, optionAction action)
	: shortName(shortName), longName(longName), takesArgument(takesArgument), action(action), alreadySet(false) {}

int Option::handle(int argc, char *argv[], int *argumentIndex, Core *core) {
	char *optionArgument = NULL;
	if (takesArgument) {
		if (*argumentIndex + 1 >= argc) {
			return CLERR_MISSING_ARGUMENT;
		}
		optionArgument = argv[++(*argumentIndex)];
	}

	if (alreadySet) {
		return CLERR_REUSED_OPTION;
	}
	if (!action) {
		return CLERR_NULL_ACTION;
	}
	if (action(optionArgument, core)) {
		return CLERR_ACTION_FAILED;
	}

	alreadySet = true;
	return 0;
}

int parseArguments(int argc, char *argv[], unsigned int numOptions, Option options[], Core *core) {
	char *argument;
	for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex) {
		argument = argv[argumentIndex];

		if ((!argument) || (*argument != '-') || (argument[1] == '\0')) {
			return CLERR_INVALID_OPTION;
		}

		// Parsing a short option or a series of short options
		if ((argument[1] != '-')) {
			int shortIndex = 1;
			bool validOption = false;

			while (argument[shortIndex]) {
				for (unsigned int i = 0; i < numOptions; ++i ) {
					if ((argument[shortIndex]) && (argument[shortIndex] == options[i].shortName)) {
						if ((options[i]).takesArgument && (argument[shortIndex + 1] != '\0')) {
							// If there are characters after an option that requires an argument, it's an error
							return CLERR_INVALID_OPTION_PLACEMENT;
						}

						validOption = true;
						int error = options[i].handle(argc, argv, &argumentIndex, core);
						if (error) { return error; }
						break;
					}
				}
				if (!validOption) {
					return CLERR_INVALID_OPTION;
				}
				++shortIndex;
			}
		}

		// Parsing a long option
		else {
			if (argument[2] == '\0') {
				// Special case: '--' marks end of options, and program does not take non-option arguments
				break;
			}

			const char* longName = &argument[2];
			bool validOption = false;

			for (unsigned int i = 0; i < numOptions; ++i) {
				if ((options[i].longName) && (!strcmp(longName, options[i].longName))) {
					validOption = true;
					int error = options[i].handle(argc, argv, &argumentIndex, core);
					if (error) { return error; }
					break;
				}
			}
			if (!validOption) {
				return CLERR_INVALID_OPTION;
			}
		}
	}
	return 0;
}