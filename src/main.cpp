#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <queue>

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
    float cost = 0.0f;
};

bool operator==(Cell a, Cell b)
{
    return a.row == b.row && a.col == b.col;
}

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

float TileCost(int type)
{
    float tileCosts[TILE_TYPE_COUNT];
    tileCosts[AIR] = 1.0f;
    tileCosts[ROCK] = 100.0f;
    tileCosts[WATER] = 25.0f;
    tileCosts[GRASS] = 10.0f;
    return tileCosts[type];
}

std::vector<Cell> FloodFill(Cell start, Cell end, Grid grid, int iterations)
{
    std::vector<Cell> result;

    bool closed[TILE_COUNT][TILE_COUNT]{};
    std::queue<Cell> open;
    open.push(start);

    bool found = false;
    //while (!open.empty())
    for (int i = 0; i < iterations; i++)
    {
        // Copy the front of the queue and remove it from the queue
        Cell front = open.front();
        open.pop();

        result.push_back(front);

        // Stop searching if we've reached our goal!
        if (front == end)
        {
            found = true;
            break;
        }

        // Indicate the cell has been explored to prevent re-exploration
        closed[front.row][front.col] = true;

        for (Cell adj : Adjacent(front, grid))
        {
            bool explored = closed[adj.row][adj.col];
            if (!explored)
            {
                open.push(adj);
            }
        }
    }

    return result;
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

    //std::queue<int> queue;
    //queue.push(1);
    //queue.push(2);
    //queue.push(3);
    //queue.push(4);
    //queue.push(5);
    //while (!queue.empty())
    //{
    //    printf("Customer number %i \n", queue.front());
    //    queue.pop();
    //}

    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    pq.push(6);
    pq.push(4);
    pq.push(7);
    pq.push(3);
    while (!pq.empty())
    {
        printf("Customer number %i \n", pq.top());
        pq.pop();
    }

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Game");
    SetTargetFPS(60);

    int iterations = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_SPACE))
            iterations++;

        Cell start{ 8, 2 }; // bottom-left
        Cell end{ 5, 5 };   // top-right
        std::vector<Cell> fill = FloodFill(start, end, tiles, iterations);

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

        for (Cell c : fill)
            DrawTile(c.row, c.col, MAGENTA);
        DrawTile(start.row, start.col, RED);
        DrawTile(end.row, end.col, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
