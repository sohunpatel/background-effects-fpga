#ifndef __CLI_H__
#define __CLI_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  char *file_in;
  char *file_out;
  bool write_out;
} CLIArgs;

CLIArgs parseArgs(int argc, char **argv);

#endif // __CLI_H__
