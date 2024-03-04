#include "app.h"
#include "cli.h"

/**
 * Main entrypoint
 */
int main(int argc, char **argv) {
  // parse cli arguments
  CLIArgs args = parseArgs(argc, argv);

  // initialize application
  app_t app;
  app_init(&app, args);
  // run main application
  app_run(&app);
  // destory and deallocated application
  app_destroy(&app);
}
