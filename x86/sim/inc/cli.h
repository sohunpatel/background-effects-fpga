#ifndef __CLI_H__
#define __CLI_H__

#include <stdbool.h>

#define UNSUPPORTED_FLAG -1
#define INPUT_NOT_PROVIDED -2
#define PROVIDED_FILE_AND_DEVICE -3

/**
 * Parser for cli arguments
 */
typedef struct {
  // input filepath that should be processed
  char *file_in;
  // if the user wants the output saved to a file, this is the file path. if
  // the user does not specify an output filepath, the pointer should be NULL
  char *file_out;
  char *dev_in;
} CLIArgs;

// help message
void help();
// argparser
CLIArgs parseArgs(int argc, char **argv);

#endif // __CLI_H__
