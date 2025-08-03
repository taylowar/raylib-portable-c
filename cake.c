
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define CAKE_EXT_CMD
#define CAKE_IMPLEMENTATION
#include "./thirdparty/cake/cake.h"

void go_rebuild_yourself(const char* src, int argc, char **argv)
{
    char *program = argv[0];
    Cake_CMD cmd = {0};
    cake_da_append(&cmd, "cc");
    cake_da_append(&cmd, "-o");
    cake_da_append(&cmd, program);
    char *output_path = cake_talloc_sprintf("%s", src); 
    cake_da_append(&cmd, output_path);
    cake_cmd_exec(cmd);
    printf("INFO: %s` rebuilt\n", program);
}

#define CAKE_REBAKE_YOURSELF(argc, argv) go_rebuild_yourself(__FILE__, argc, argv)

void go_rebuild_baker()
{
    Cake_CMD cmd = {0};
    cake_da_append(&cmd, "cc");
    cake_da_append(&cmd, "-o");
    cake_da_append(&cmd, "cake-linux");
    cake_da_append(&cmd, "./src-build/cake_linux.c");
    cake_cmd_exec(cmd);
    printf("INFO: `./cake-linux` rebuilt\n");
}

int main(int argc, char **argv)
{
    CAKE_REBAKE_YOURSELF(argc, argv);
    go_rebuild_baker();
    return 0;
}
