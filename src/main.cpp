#include "raylib.h"
#include "Math.h"

bool PointInRec(Vector2 point, Rectangle rec)
{
    // Fill in this function to determine if a point is inside a rectangle
    // A point is inside a rectangle if:
    // point x > rec x && point x < rec x + rec width &&
    // point y > rec y && point y < rec y + rec height
    // For more information, see https://www.jeffreythompson.org/collision-detection/point-rect.php
    return false;
}

int main()
{
    Rectangle rec;
    rec.x = 200;
    rec.y = 200;
    rec.width = 160;
    rec.height = 80;

    InitWindow(800, 800, "Game");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        Color color = PointInRec(mouse, rec) ? RED : GREEN;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(mouse, 3.0f, color);
        DrawRectangleRec(rec, color);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
