#include "cli.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void help() {
  const char *help_message =
      "background-effects-sim\n"
      "Simulation model for FPGA image processing engine\n"
      "\n"
      "USAGE:\n"
      "    background-effects-sim [FLAGS]\n"
      "\n"
      "FLAGS:\n"
      "    -h                print this help message\n"
      "    -i <INPUT_FILE>   input mp4 file to read video from\n"
      "    -o <OUTPUT_FILE>  output mp4 file to write to\n"
      "    -d <DEVICE>       /dev/videox path to read frames from";

  printf("%s\n", help_message);
}

/**
 * Argument parser for application
 *   @param[in]  argc  number of arguments from standard main function signature
 *   @param[in]  argv  vector of arguments from standard main function signature
 */
CLIArgs parseArgs(int argc, char **argv) {
  // return code
  int rc = 0;

  // deafult arguments are NULL for the filepaths
  CLIArgs args = {.file_in = NULL, .file_out = NULL, .dev_in = NULL};

  // filepath indeces are used to get teh correct char* from argv
  int filein_idx = -1;
  int fileout_idx = -1;
  int dev_idx = -1;

  // iterate through the args in argv
  for (int i = 0; i < argc; i++) {
    // copy the current operation to an option variable
    char const *option = argv[i];
    // if the argument starts with a dash, we not that it is a flag
    if (option[0] == '-') {
      // right now, i am only supporting single char flags. so, the second
      // char in option would be the flag. we then know that the next arg
      // in argv will be the index of the matching argument
      switch (option[1]) {
      case 'i':
        // input filepath
        filein_idx = i + 1;
        i++;
        break;
      case 'o':
        // output filepath
        fileout_idx = i + 1;
        i++;
        break;
      case 'd':
        // input /dev/videox
        dev_idx = i + 1;
        i++;
        break;
      case 'h':
        // help message
        help();
        _exit(0);
        break;
      default:
        // if this is an unsupported flag, set the return code to
        // UNSUPPORTED_FLAG
        rc = UNSUPPORTED_FLAG;
        fprintf(stderr, "%s is not a supported flag\n", option);
        break;
      }
    }
  }

  // if filein_idx is within the appropriate bounds, we can get the input
  // filepath from argv
  if (filein_idx > -1 && filein_idx <= argc) {
    if (strcmp(strrchr(argv[filein_idx], '.') + 1, ".mp4")) {
      args.file_in = argv[filein_idx];
    }
  }

  // if fileout_idx is withing the appropriate bounds, we can get the output
  // filepath from argv
  if (fileout_idx > -1 && fileout_idx <= argc) {
    if (strcmp(strrchr(argv[fileout_idx], '.') + 1, ".mp4")) {
      args.file_in = argv[fileout_idx];
    }
  }

  // if dev_idx is within the appropriate bounds, we can get the correct device
  // from argv
  if (dev_idx > -1 && dev_idx <= argc) {
    args.dev_in = argv[dev_idx];
  }

  // if the input filepath is not provided, the application needs to exit with
  // an error code
  if (!args.file_in || !args.dev_in) {
    rc = INPUT_NOT_PROVIDED;
    fprintf(stderr, "please provide an input file or device: -i [filename] OR "
                    "-d [/dev/videox]");
  }

  if (args.file_in && args.dev_in) {
    rc = PROVIDED_FILE_AND_DEVICE;
    fprintf(stderr, "cannot provide both an input file and device\n");
  }

  // if there is an error, exit the application
  if (rc != 0) {
    help();
    _exit(rc);
  }

  // return parsed cli arguments
  return args;
}
