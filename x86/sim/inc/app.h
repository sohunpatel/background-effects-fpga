#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>

#include "cli.h"

typedef struct {
  FILE *file_in;
  FILE *file_out;
} app_t;

void app_init(app_t *me, CLIArgs args);

void app_destroy(app_t *me);

#endif // __APP_H__
