#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>

#include "cli.h"
#include "mp4.h"

/**
 * This structure describes the required applciation data.
 */
typedef struct {
  // video file read to run background effect algorithms
  char *filepath_in;
  // if the user specifies an output file, this file will be created and
  // written to
  char *filepath_out;
  // handler for input MP4 file
  mp4_t input_file;
  // handler for output MP4 file
  mp4_t output_file;
} app_t;

// constructor
void app_init(app_t *me, CLIArgs args);
// deallocator
void app_destroy(app_t *me);
// run thread handling
void app_run(app_t *me);

#endif // __APP_H__
