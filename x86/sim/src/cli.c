#include "cli.h"
#include <stdio.h>
#include <unistd.h>

CLIArgs parseArgs(int argc, char **argv) {
  int rc = 0;

  CLIArgs args = {
    .file_in = NULL,
    .file_out = NULL,
    .write_out = false
  };

  int filein_idx = -1;
  int fileout_idx = -1;

  for (int i = 0; i < argc; i++) {
    char const *option = argv[i];
    if (option[0] == '-') {
      switch (option[1]) {
      case 'i':
        filein_idx = i + 1;
        i++;
        break;
      case 'o':
        fileout_idx = i + 1;
        args.write_out = true;
        i++;
        break;
      default:
        rc = -1;
        fprintf(stderr, "%s is not a supported flag\n", option);
        break;
      }
    }
  }

  if (filein_idx > -1 && filein_idx <= argc) {
    args.file_in = argv[filein_idx];
  }

  if (fileout_idx > -1 && fileout_idx <= argc) {
    args.file_out = argv[fileout_idx];
  }

  if (!args.file_in) {
    rc = -2;
    fprintf(stderr, "please provide an input file: -i [filename]");
  }

  if (!args.file_out && args.write_out) {
    rc = -3;
    fprintf(stderr, "%s does not exist", argv[fileout_idx]);
  }

  if (rc != 0) {
    _exit(rc);
  }

  return args;
}
