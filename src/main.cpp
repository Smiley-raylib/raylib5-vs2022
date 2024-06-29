#include "raylib.h"
#include "Math.h"

int main()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
