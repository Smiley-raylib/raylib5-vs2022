#include "Assets.h"
#include "Constants.h"

#include "Weapons.h"
#include "Systems.h"
#include "Steering.h"
#include "Timer.h"

struct Player
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    float moveSpeed = 0.0f;

    int weaponType = RIFLE;
    Timer shootTimer;
};

void Shoot(Vector2 position, Vector2 direction, float radius, Projectiles& projectiles, int type)
{
    switch (type)
    {
        case RIFLE:
        {
            Projectile rifle = ShootRifle(position, radius, direction);
            projectiles.rifle.push_back(rifle);
        }
        break;

        case SHOTGUN:
        {
            std::array<Projectile, 3> shotgun = ShootShotgun(position, radius, direction);
            for (Projectile& p : shotgun)
                projectiles.shotgun.push_back(p);
        }
        break;

        case MACHINE_GUN:
        {
            Projectile machine = ShootMachineGun(position, radius, direction);
            projectiles.machineGun.push_back(machine);
        }
        break;

        case AKIMBO:
        {
            std::array<Projectile, 2> akimbo = ShootAkimbo(position, radius, direction);
            for (Projectile& p : akimbo)
                projectiles.akimbo.push_back(p);
        }
        break;

        case ROCKET:
        {
            std::array<Projectile, 5> rocket = ShootRocket(position, radius, direction);
            for (Projectile& p : rocket)
                projectiles.rocket.push_back(p);
        }
    }
}

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

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubble Arena");
    SetTargetFPS(60);
    LoadAssets();

    Map map;
    GenMap(map);

    Player player;
    player.moveSpeed = SCREEN_SIZE;

    player.weaponType = ROCKET;
    player.shootTimer.total = COOLDOWN_SHOTGUN;

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
                Shoot(player.pos, mouseDirection, RADIUS_PLAYER, projectiles, player.weaponType);
            }
        }
        
        UpdateProjectiles(projectiles, map, dt);
        PruneProjectiles(projectiles);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        DrawSceenEdges();
        DrawProjectiles(projectiles);
        DrawPlayer(player.pos);
        for (const Obstacle& o : map.obstacles)
        {
            DrawCircleV(o.pos, o.radius, o.color);
        }

        EndMode2D();
        EndDrawing();
    }

    UnloadAssets();
    CloseWindow();
    return 0;
}
