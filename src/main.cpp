#include <raylib.h>
#include <raymath.h>
#include <vector>

const int SCREEN_SIZE = 800;
const int TILE_COUNT = 10;
const int TILE_SIZE = SCREEN_SIZE / TILE_COUNT;

using Grid = int[TILE_COUNT][TILE_COUNT];

enum TileType
{
    AIR,
    ROCK,
    WATER,
    GRASS,
    TILE_TYPE_COUNT
};

struct Cell
{
    int row = -1;
    int col = -1;
};

std::vector<Cell> Adjacent(Cell cell, Grid grid)
{
    std::vector<Cell> adjacent;
    Cell left = { cell.row, cell.col - 1 };
    Cell right = { cell.row, cell.col + 1 };
    Cell up = { cell.row - 1, cell.col };
    Cell down = { cell.row + 1, cell.col };
    if (left.col >= 0) adjacent.push_back(left);
    if (right.col < TILE_COUNT) adjacent.push_back(right);
    if (up.row >= 0) adjacent.push_back(up);
    if (down.row < TILE_COUNT) adjacent.push_back(down);
    return adjacent;
}

void DrawTile(int row, int col, Color color)
{
    DrawRectangle(TILE_SIZE * col, TILE_SIZE * row, TILE_SIZE, TILE_SIZE, color);
}

Color TileColor(int type)
{
    Color tileColors[TILE_TYPE_COUNT];
    tileColors[AIR] = WHITE;
    tileColors[ROCK] = GRAY;
    tileColors[WATER] = SKYBLUE;
    tileColors[GRASS] = LIME;
    return tileColors[type];
}

int main()
{
    Grid tiles =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 2, 2, 2, 2, 0, 0, 1 },
        { 1, 0, 0, 3, 3, 3, 3, 0, 0, 1 },
        { 1, 0, 0, 3, 0, 0, 3, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Game");
    SetTargetFPS(60);

    Cell test{ 4, 2 };

    while (!WindowShouldClose())
    {
        std::vector<Cell> adjacent = Adjacent(test, tiles);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int row = 0; row < TILE_COUNT; row++)
        {
            for (int col = 0; col < TILE_COUNT; col++)
            {
                int type = tiles[row][col];
                Color color = TileColor(type);
                DrawTile(row, col, color);
            }
        }

        DrawTile(test.row, test.col, MAGENTA);
        for (Cell adj : adjacent)
            DrawTile(adj.row, adj.col, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
