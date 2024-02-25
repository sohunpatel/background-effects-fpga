#include "app.h"
#include "cli.h"
#include "image_buffer.h"

/**
 * Main entrypoint
 */
int main(int argc, char **argv) {
  // parse cli arguments
  CLIArgs args = parseArgs(argc, argv);

  // declare circular image buffer and initialize it
  image_buffer_t buffer;
  image_buffer_init(&buffer, 10);
  // destroy image buffer
  image_buffer_destroy(&buffer);

  // initialize application
  app_t app;
  app_init(&app, args);
  // run main application
  app_run(&app);
  // destory and deallocated application
  app_destroy(&app);
}
