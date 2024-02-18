#include "app.h"

void app_init(app_t *me, CLIArgs args) {
  me->file_in = fopen(args.file_in, "r");
  if (args.file_out) {
    me->file_out = fopen(args.file_out, "w");
  } else {
    me->file_out = NULL;
  }
}

void app_destroy(app_t *me) {
  fclose(me->file_in);
  fclose(me->file_out);
}
