#include <raylib.h>
#include <raymath.h>

const int SCREEN_SIZE = 800;
const int TILE_COUNT = 10;
const int TILE_SIZE = SCREEN_SIZE / TILE_COUNT;

void DrawTile(int row, int col, Color color)
{
    DrawRectangle(TILE_SIZE * col, TILE_SIZE * row, TILE_SIZE, TILE_SIZE, color);
}

int main()
{
    int tiles[TILE_COUNT][TILE_COUNT] =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 1, 1, 1, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Game");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int row = 0; row < TILE_COUNT; row++)
        {
            for (int col = 0; col < TILE_COUNT; col++)
            {
                Color color = tiles[row][col] == 1 ? GRAY : WHITE;
                DrawTile(row, col, color);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
