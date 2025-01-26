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
            Projectile rocket = ShootRocket(position, radius, direction);
            projectiles.rocket.push_back(rocket);
        }
    }
}

void DrawPlayer(Vector2 pos)
{
    DrawTexCircle(gTexBubble, pos, RADIUS_PLAYER, BLUE);
}

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubble Arena");
    SetTargetFPS(60);
    LoadAssets();

    Map map;
    map.obstacles.resize(1024);
    for (Obstacle& o : map.obstacles)
    {
        o.pos.x = Random(WORLD_MIN, WORLD_MAX);
        o.pos.y = Random(WORLD_MIN, WORLD_MAX);
        o.radius = Random(10.0f, 25.0f);
        o.color = COLORS[rand() % COLORS.size()];
    }

    Player player;
    player.moveSpeed = SCREEN_SIZE * 0.5f;

    player.weaponType = ROCKET;
    player.shootTimer.total = COOLDOWN_SHOTGUN;

    Camera2D camera;
    camera.target = player.pos;
    camera.offset = Vector2Ones * SCREEN_SIZE * 0.5f;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

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
        DrawProjectiles(projectiles);
        DrawPlayer(player.pos);
        for (const Obstacle& o : map.obstacles)
        {
            DrawCircleV(o.pos, o.radius, o.color);
        }

        const float spread = PI / 5.0f;
        float angle = -spread * 2.0f;
        float r = 100.0f;
        for (int i = 0; i < 5; i++)
        {

            Vector2 dir = Vector2Rotate(mouseDirection * -1.0f, angle) * r;
            DrawCircleV(player.pos + dir, 20.0f, RED);
            angle += spread;
        }

        // Since everything is relative to the camera, just use world-space UI!
        //DrawText(TextFormat("mx: %f my: %f", mouse.x, mouse.y), player.pos.x, player.pos.y - 50.0f, 20, BLUE);
        EndMode2D();
        DrawFPS(10, 10);
        //projectiles.rifle.size();
        //int  = projectiles.rifle.size() + projectiles.rifle.size() + projectiles.rifle.size() + projectiles.rifle.size() + projectiles.rifle.size();
        DrawText(TextFormat("Bullet count: %i", projectiles.rifle.size()), 10, 30, 20, RED);
        EndDrawing();
    }

    UnloadAssets();
    CloseWindow();
    return 0;
}
