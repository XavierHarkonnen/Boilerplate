#ifndef LIB_CL_INPUT_PARSE_ARGUMENTS_HPP
#define LIB_CL_INPUT_PARSE_ARGUMENTS_HPP

#include <string.h>
#include <stdio.h>

#include "../core/core.hpp"

#define CLERR_START_VALUE -255

enum CommandLineError {
	CLERR_INVALID_OPTION = CLERR_START_VALUE,
	CLERR_REUSED_OPTION,
	CLERR_MISSING_ARGUMENT,
	CLERR_NULL_ACTION,
	CLERR_ACTION_FAILED,
	CLERR_INVALID_OPTION_PLACEMENT
};

/**
 * @brief Typedefed function; to be called by command line arguments. Must return 0 if successful.
 */
typedef int (*optionAction)(const char *argument, Core *core);

class Option {
public:
	/**
	 * @brief Constructs an Option object
	 * 
	 * @param shortName The single character used to call an option ('\0' to avoid)
	 * @param longName The string used to call an option (NULL to avoid)
	 * @param takesArgument Whether the option requires a single argument
	 * @param action A function pointer to perform the option's corresponding function
	 */
	Option(char shortName, const char *longName, bool takesArgument, optionAction action);

	friend int parseArguments(int argc, char *argv[], unsigned int numOptions, Option options[], Core *core);

private:
	const char shortName; // '\0' is interpreted as empty
	const char *longName; // NULL is interpreted as empty
	bool takesArgument;
	optionAction action; // Action (function) corresponding to argument
	bool alreadySet; // Used to determine if option has already been set

	/**
	 * @brief Handles the validation and execution of an option.
	 * 
	 * @param argc The number of command line arguments
	 * @param argv The vector of command line arguments
	 * @param argumentIndex The current argument being parsed as an option in argv
	 * @param core The core of the program upon which the options will act
	 * @return CLERR_MISSING_ARGUMENT if an option requiring an argument is missing its argument, CLERR_REUSED_OPTION if
	 * an option is used twice, CLERR_NULL_ACTION if an option does not have an action, or CLERR_ACTION_FAILED if the
	 * option's action returns a nonzero value
	 */
	int handle(int argc, char *argv[], int *argumentIndex, Core *core);
};

/**
 * @brief Parses command line arguments according to a provided set of options.
 * 
 * @param argc The number of command line arguments
 * @param argv The vector of command line arguments
 * @param numOptions The number of options in the options array
 * @param options An array of options corresponding to command-line arguments
 * @param core The core of the program upon which the options will act
 * @return CLERR_INVALID_OPTION if an invalid option is encountered, CLERR_INVALID_OPTION_PLACEMENT if an option with
 * required arguments is not last in its series, CLERR_MISSING_ARGUMENT if an option requiring an argument is missing
 * its argument, CLERR_REUSED_OPTION if an option is used twice, CLERR_NULL_ACTION if an option does not have an
 * action, or CLERR_ACTION_FAILED if the option's action returns a nonzero value
 */
int parseArguments(int argc, char *argv[], unsigned int numOptions, const Option options[], Core *core);

#endif