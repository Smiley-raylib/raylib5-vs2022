#include "Assets.h"
#include "Constants.h"
#include "Systems.h"

void DrawPlayer(Vector2 pos)
{
    DrawTexCircle(gTexBubble, pos, RADIUS_PLAYER, BLUE);
}

void GenMap(Map& map)
{
    Obstacle top, bot, left, right;
    Obstacle topLeft, topRight, botLeft, botRight;

    top.pos = Vector2UnitY * 1000.0f;
    bot.pos = Vector2UnitY * -1000.0f;
    left.pos = Vector2UnitX * -1000.0f;
    right.pos = Vector2UnitX * 1000.0f;

    topLeft.pos = top.pos * 1.5f + left.pos * 1.5f;
    topRight.pos = top.pos * 1.5f + right.pos * 1.5f;
    botLeft.pos = bot.pos * 1.5f + left.pos * 1.5f;
    botRight.pos = bot.pos * 1.5f + right.pos * 1.5f;

    top.radius = bot.radius = left.radius = right.radius = 500.0f;
    topLeft.radius = botRight.radius = botLeft.radius = botRight.radius = 500.0f;
    top.color = bot.color = left.color = right.color = SKYBLUE;
    topLeft.color = botRight.color = botLeft.color = botRight.color = SKYBLUE;

    std::vector<Obstacle>& o = map.obstacles;
    o.push_back(top);
    o.push_back(bot);
    o.push_back(left);
    o.push_back(right);
    o.push_back(topLeft);
    o.push_back(topRight);
    o.push_back(botLeft);
    o.push_back(botRight);
}

void DrawSceenEdges()
{
    Rectangle edges;
    edges.x = WORLD_MIN;
    edges.y = WORLD_MIN;
    edges.width = WORLD_MAX * 2.0f;
    edges.height = WORLD_MAX * 2.0f;
    DrawRectangleLinesEx(edges, 10.0f, DARKBLUE);
}

enum Screen
{
    GAME,
    WIN,
    LOSE
};

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubble Arena");
    SetTargetFPS(60);
    LoadAssets();

    Map map;
    GenMap(map);

    Player player;
    player.moveSpeed = SCREEN_SIZE;

    player.weaponType = RIFLE;
    player.shootTimer.total = COOLDOWN_SHOTGUN;

    Enemies enemies;
    enemies.resize(10);
    float xStart =  + 250.0f;
    for (int i = 0; i < enemies.size(); i++)
    {
        Enemy& enemy = enemies[i];
        enemy.pos.x = WORLD_MIN + 250.0f + (i * 500.0f);
        enemy.pos.y = WORLD_MAX - 250.0f;
        enemy.weaponType = rand() % WEAPON_COUNT;
        enemy.shootTimer.total = WeaponCooldown(enemy.weaponType);
    }

    Camera2D camera;
    camera.target = player.pos;
    camera.offset = Vector2Ones * SCREEN_SIZE * 0.5f;
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;

    Projectiles projectiles;
    projectiles.rifle.reserve(128);
    projectiles.shotgun.reserve(128);
    projectiles.machineGun.reserve(128);
    projectiles.akimbo.reserve(128);
    projectiles.rocket.reserve(128);

    Screen screen = GAME;

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        const Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
        const Vector2 mouseDirection = Vector2Normalize(mouse - player.pos);

        Vector2 moveDir = Vector2Zeros;
        if (IsKeyDown(KEY_W))
        {
            moveDir -= Vector2UnitY;
        }
        if (IsKeyDown(KEY_S))
        {
            moveDir += Vector2UnitY;
        }
        if (IsKeyDown(KEY_A))
        {
            moveDir -= Vector2UnitX;
        }
        if (IsKeyDown(KEY_D))
        {
            moveDir += Vector2UnitX;
        }

        moveDir = Vector2Normalize(moveDir);
        player.vel = moveDir * player.moveSpeed;
        player.pos += player.vel * dt;
        player.pos = Vector2Clamp(player.pos, Vector2Ones * WORLD_MIN, Vector2Ones * WORLD_MAX);
        camera.target = player.pos;

        if (IsKeyPressed(KEY_LEFT_SHIFT))
        {
            ++player.weaponType %= WEAPON_COUNT;
            player.shootTimer.total = WeaponCooldown(player.weaponType);
        }

        Tick(player.shootTimer, dt);
        if (Expired(player.shootTimer))
        {
            if (IsKeyDown(KEY_SPACE))
            {
                Reset(player.shootTimer);
                Shoot(player.pos, mouseDirection, RADIUS_PLAYER, projectiles, player.weaponType, PLAYER);
            }
        }
        
        UpdateProjectiles(player, enemies, projectiles, map, dt);
        UpdateEnemies(player, enemies, projectiles, dt);
        PruneProjectiles(projectiles);
        PruneEnemies(enemies);
        ResolveMapCollisions(player, enemies, map);

        if (player.health <= 0.0f)
        {
            screen = LOSE;
        }
        else if (enemies.empty())
        {
            screen = WIN;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == GAME)
        {
            BeginMode2D(camera);
            DrawSceenEdges();
            DrawProjectiles(projectiles);
            DrawPlayer(player.pos);

            for (const Enemy& enemy : enemies)
            {
                DrawCircleV(enemy.pos, RADIUS_ENEMY, WeaponColor(enemy.weaponType));
            }

            for (const Obstacle& o : map.obstacles)
            {
                DrawCircleV(o.pos, o.radius, o.color);
            }
            EndMode2D();
        }
        else if (screen == WIN)
        {
            const char* text = "YOU WIN :)";
            int x = MeasureText(text, 32);
            DrawText(text, SCREEN_SIZE * 0.5f - x * 0.5f, SCREEN_SIZE * 0.5, 32, GREEN);
        }
        else if (screen == LOSE)
        {
            const char* text = "YOU LOSE :(";
            int x = MeasureText(text, 32);
            DrawText(text, SCREEN_SIZE * 0.5f - x * 0.5f, SCREEN_SIZE * 0.5, 32, RED);
        }
        EndDrawing();
    }

    UnloadAssets();
    CloseWindow();
    return 0;
}
