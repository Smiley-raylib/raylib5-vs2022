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

namespace std
{
    template<>
    struct greater<Cell>
    {
        bool operator()(Cell a, Cell b)
        {
            return a.cost > b.cost;
        }
    };
}

struct Node
{
    Cell curr;
    Cell prev;
    float cost = 0.0f;
};

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

std::vector<Cell> Retrace(Node nodes[TILE_COUNT][TILE_COUNT], Cell start, Cell end)
{
    std::vector<Cell> path;
    Cell curr = end;
    Cell prev = nodes[curr.row][curr.col].prev;

    while (prev.row != -1 && prev.col != -1)
    {
        path.push_back(curr);
        curr = prev;
        prev = nodes[curr.row][curr.col].prev;
    }

    return path;
}

std::vector<Cell> Dijkstra(Cell start, Cell end, Grid grid, int iterations)
{
    std::vector<Cell> searched;

    Node nodes[TILE_COUNT][TILE_COUNT];
    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            nodes[row][col].curr = { row, col };
            nodes[row][col].cost = FLT_MAX;
        }
    }

    std::priority_queue<Cell, std::vector<Cell>, std::greater<Cell>> open;
    start.cost = 0.0f;
    open.push(start);
    nodes[start.row][start.col].cost = start.cost;

    bool found = false;
    //while (!open.empty()) <-- Replace for-loop with this to make this search complete automatically!
    for (int i = 0; i < iterations; i++)
    {
        // Copy the front of the queue and remove it from the queue
        Cell front = open.top();
        open.pop();
        
        // Store explored nodes for debug rendering
        searched.push_back(front);
    
        // Stop searching if we've reached our goal!
        if (front == end)
        {
            found = true;
            break;
        }
    
        for (Cell adj : Adjacent(front, grid))
        {
            float prevCost = nodes[adj.row][adj.col].cost;
            float currCost = nodes[front.row][front.col].cost + TileCost(grid[adj.row][adj.col]);

            if (currCost < prevCost)
            {
                adj.cost = currCost;
                open.push(adj);
                nodes[adj.row][adj.col].cost = currCost;
                nodes[adj.row][adj.col].prev = front;
            }
        }
    }

    // Output the path if we've found one, otherwise output our search so far
    return found ? Retrace(nodes, start, end) : searched;/*std::vector<Cell>{}*/;
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

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Dijkstra's Algorithm");
    SetTargetFPS(60);

    int iterations = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_SPACE))
            iterations++;

        if (IsKeyDown(KEY_LEFT_SHIFT))
            iterations--;

        Cell start{ 8, 2 };
        Cell end{ 2, 8 };
        std::vector<Cell> path = Dijkstra(start, end, tiles, iterations);

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

        for (Cell cell : path)
            DrawTile(cell.row, cell.col, MAGENTA);
        DrawTile(start.row, start.col, RED);
        DrawTile(end.row, end.col, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
