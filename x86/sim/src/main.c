#include <stdio.h>

#include "app.h"
#include "cli.h"
#include "image_buffer.h"

int main(int argc, char **argv) {
    CLIArgs args = parseArgs(argc, argv);

    image_buffer_t buffer;
    image_buffer_init(&buffer);
    image_buffer_destroy(&buffer);

    app_t app;
    app_init(&app, args);
    app_run(&app);
    app_destroy(&app);
}
