#include "raylib.h"
#include "Math.h"

int main()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);
    float w = GetScreenWidth();
    float h = GetScreenHeight();
    float tc = 10.0f;

    while (!WindowShouldClose())
    {
        // Draw a line from the top-left to the top-right of our screen!
        Vector2 A = { 0.0f, 0.0f };
        Vector2 B = { w, 0.0f };

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello World!", 10, 10, 20, GRAY);
        DrawLineEx(A, B, tc, VIOLET);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
