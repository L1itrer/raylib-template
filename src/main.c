#include "raylib.h"

#define WINDOW_FACTOR 2
#define WINDOW_WIDTH (1920/(WINDOW_FACTOR))
#define WINDOW_HEIGHT (1080/(WINDOW_FACTOR))

const char* WINDOW_TITLE = "Raylib template";

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(PINK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
