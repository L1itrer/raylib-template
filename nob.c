#define NOB_IMPLEMENTATION
#include "nob.h"

#define OUTPUT_DIR "./out/"

#ifdef _WIN32
#   define BUILD_RESULT (OUTPUT_DIR "game.exe")
#   define BUILD_RAYLIB (OUTPUT_DIR "raylib.lib")
#else
#   define BUILD_RESULT (OUTPUT_DIR "game")
#   define BUILD_RAYLIB (OUTPUT_DIR "raylib.a")
#endif

#define RAYLIB_DIR "./src/raylib5-5/"

const char* raylib_files[] = {
    "raudio",
    "rcore",
    "rglfw",
    "rmodels",
    "rshapes",
    "rtext",
    "rtextures",
    "utils",
};


const char* raylib_files_full[] = {
    RAYLIB_DIR "raudio.c",
    RAYLIB_DIR "rcore.c",
    RAYLIB_DIR "rglfw.c",
    RAYLIB_DIR "rmodels.c",
    RAYLIB_DIR "rshapes.c",
    RAYLIB_DIR "rtext.c",
    RAYLIB_DIR "rtextures.c",
    RAYLIB_DIR "utils.c",
    RAYLIB_DIR "config.h",
    RAYLIB_DIR "rgestures.h",
    RAYLIB_DIR "raymath.h",
    RAYLIB_DIR "rlgl.h",
    RAYLIB_DIR "rcamera.h",
    RAYLIB_DIR "raylib.h",
    RAYLIB_DIR "utils.h",
};

int main(int argc, char* argv[])
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd_static = {0};
    Cmd* cmd = &cmd_static;

    bool rebuild_deps = false;

    for (int i = 1; i < argc;++i)
    {
        if (strcmp(argv[i], "deps") == 0)
        {
            rebuild_deps = true;
        }
    }

    if (!nob_mkdir_if_not_exists("./out")) return 1;

    if (nob_needs_rebuild(BUILD_RAYLIB, raylib_files_full, NOB_ARRAY_LEN(raylib_files_full)) || rebuild_deps)
    {
        nob_log(INFO, "raylib requires rebuild");
        for (int i = 0; i < NOB_ARRAY_LEN(raylib_files);++i)
        {
            cmd_append(cmd, "clang-cl");
            cmd_append(cmd, "-g");
            cmd_append(cmd, "-W0");
            cmd_append(cmd, "-I./src/raylib5-5/external/glfw/include/");
            cmd_append(cmd, "-DPLATFORM_DESKTOP");
            cmd_append(cmd, nob_temp_sprintf("./src/raylib5-5/%s.c", raylib_files[i]));
            cmd_append(cmd, "-c");
            cmd_append(cmd, nob_temp_sprintf("-Fo:./out/%s.obj", raylib_files[i]));
            if (!cmd_run(cmd)) return 1;
        }

        cmd_append(cmd, "lib");
        for (int i = 0; i < NOB_ARRAY_LEN(raylib_files);++i)
        {
            cmd_append(cmd, nob_temp_sprintf("./out/%s.obj", raylib_files[i]));
        }
        cmd_append(cmd, "-out:./out/raylib.lib");
        if (!cmd_run(cmd)) return 1;
    }

    nob_cmd_append(cmd, "clang-cl");

    nob_cmd_append(cmd, "./src/main.c", "./out/raylib.lib");
    nob_cmd_append(cmd, "-I./src/raylib5-5/");
    nob_cmd_append(cmd, "user32.lib", "kernel32.lib", "gdi32.lib", "shell32.lib");
    nob_cmd_append(cmd, "-W4", "-g");
    nob_cmd_append(cmd, "-o", BUILD_RESULT);

    if (!cmd_run(cmd)) return 1;

  return 0;
}
