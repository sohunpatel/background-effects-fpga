#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>

#include "cli.h"
#include "mp4.h"

typedef struct {
    char *filepath_in;
    char *filepath_out;
    mp4_t input_file;
    mp4_t output_file;
} app_t;

void app_init(app_t *me, CLIArgs args);
void app_destroy(app_t *me);
void app_run(app_t *me);

#endif // __APP_H__
