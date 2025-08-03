#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define CAKE_EXT_CMD
#define CAKE_IMPLEMENTATION
#include "../thirdparty/cake/cake.h"

void make_build_directory()
{
    Cake_CMD mkdir_cmd = {0};
    cake_cmd_push(&mkdir_cmd, "mkdir", "-p", "./build");
    pid_t cpid = cake_cmd_exec(mkdir_cmd);
    if (!cake_cmd_wait(cpid)) {
        printf("ERROR: Unable to create './build' directory\n");
        exit(1);
    }
    printf("INFO: Created './build' directory\n");
}

void build_raylib()
{
    Cake_CMD mkdir_cmd = {0};
    cake_cmd_push(&mkdir_cmd, "mkdir", "-p", "./build/raylib");
    pid_t cpid = cake_cmd_exec(mkdir_cmd);
    if (!cake_cmd_wait(cpid)) {
        fprintf(stderr, "ERROR: Unable to create './build/raylib' directory\n");
        exit(1);
    }
    printf("INFO: Created './build/raylib' directory\n");
    const char* raylib_modules[] = {
        "rcore",
        "raudio",
        "rglfw",
        "rmodels",
        "rshapes",
        "rtext",
        "rtextures",
        "utils"
    };
    size_t n = 8;
    const char* input_path_base = "./thirdparty/raylib/raylib-5.6/src";
    for (size_t i=0; i<n;++i) {
        Cake_CMD cmd = {0};
        char *input_path = cake_talloc_sprintf(
            "%s/%s.c",
            input_path_base,
            raylib_modules[i]
        ); 
        char *output_path = cake_talloc_sprintf("./build/raylib/%s.o", raylib_modules[i]); 
        cake_cmd_push(&cmd, "cc");
        cake_cmd_push(&cmd, "-ggdb");
        cake_cmd_push(&cmd, "-DPLATFORM_DESKTOP");
        cake_cmd_push(&cmd, "-fPIC");
        cake_cmd_push(&cmd, "-DSUPPORT_FILEFORMAT_FLAC=1");
        cake_cmd_push(&cmd, "-I./thirdparty/raylib/raylib-5.6/src/external/glfw/include");
        cake_cmd_push(&cmd, "-c");
        cake_cmd_push(&cmd, input_path);
        cake_cmd_push(&cmd, "-o");
        cake_cmd_push(&cmd, output_path);
        cake_cmd_wait(cake_cmd_exec(cmd));
        printf("INFO: `%s` compiled\n", raylib_modules[i]);
    }
    Cake_CMD cmd = {0};
    cake_cmd_push(&cmd, "cc");
    cake_cmd_push(&cmd, "-shared");
    cake_cmd_push(&cmd, "-o");
    cake_cmd_push(&cmd, "./build/raylib/libraylib.so");
    for (size_t i=0; i<n;++i) {
        char* output_path = cake_talloc_sprintf("./build/raylib/%s.o", raylib_modules[i]); 
        cake_cmd_push(&cmd, output_path);
    }
    cake_cmd_push(&cmd, "-lm");
    cake_cmd_wait(cake_cmd_exec(cmd));
    printf("INFO: `./libraylib.so` linked\n");
}

void build_app(const char* app_name)
{
    Cake_CMD cmd = {0};
    cake_cmd_push(&cmd, "clang");
    cake_cmd_push(&cmd, "-Wall");
    cake_cmd_push(&cmd, "-Wextra");
    cake_cmd_push(&cmd, "-I.");
    cake_cmd_push(&cmd, "-I./thirdparty/raylib/raylib-5.6/src");
    cake_cmd_push(&cmd, "-o");
    cake_cmd_push(&cmd, cake_talloc_sprintf("./build/%s", app_name));
    cake_cmd_push(&cmd, cake_talloc_sprintf("./src/%s.c", app_name));
    cake_cmd_push(&cmd, "-lm");
    cake_cmd_push(&cmd, "-Wl,-rpath=./build/");
    cake_cmd_push(&cmd, "-Wl,-rpath=.");
    cake_cmd_push(&cmd, "-Wl,-rpath=./build/raylib");
    cake_cmd_push(&cmd, "-Wl,-rpath=./raylib");
    cake_cmd_push(&cmd, "-L./build/raylib");
    cake_cmd_push(&cmd, "-l:libraylib.so");
    cake_cmd_wait(cake_cmd_exec(cmd));
    printf("INFO: `./build/%s` baked\n", app_name);
    cake_talloc_reset();
}

int main(void)
{
    make_build_directory();
    build_raylib();
    build_app("main");
    return 0;
}
