#include <raylib.h>
#include "Colors.h"
#include "Weapons.h"
#include "Steering.h"
#include "Timer.h"
#include "Systems.h"

constexpr Rectangle WORLD_REC{ WORLD_MIN, WORLD_MIN, WORLD_MAX * 2.0f, WORLD_MAX * 2.0f };

struct Player
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    float moveSpeed = 0.0f;

    int weaponType = RIFLE;
    Timer shootTimer;
};

// Test object
struct Ball
{
    Vector2 pos = Vector2Zeros;
    Color color = BLACK;
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
    }
}

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubble Arena");
    SetTargetFPS(60);

    std::vector<Ball> balls(128);
    for (Ball& ball : balls)
    {
        ball.pos.x = Random(WORLD_MIN, WORLD_MAX);
        ball.pos.y = Random(WORLD_MIN, WORLD_MAX);
        ball.color = COLORS[rand() % COLORS.size()];
    }

    Player player;
    player.pos = Vector2Ones * SCREEN_SIZE * 0.5f;
    player.moveSpeed = SCREEN_SIZE * 0.5f;

    player.weaponType = RIFLE;
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

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        const Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);

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
                Vector2 mouseDirection = Vector2Normalize(mouse - player.pos);
                Reset(player.shootTimer);
                Shoot(player.pos, mouseDirection, RADIUS_PLAYER, projectiles, player.weaponType);
            }
        }
        
        // Bullet physics update
        for (Projectile& p : projectiles.rifle)
        {
            p.pos += p.vel * dt;

            for (const Ball& b : balls)
            {
                p.destroy |= CheckCollisionCircles(p.pos, RADIUS_RIFLE, b.pos, RADIUS_BALL);
            }
        }

        for (Projectile& p : projectiles.shotgun)
        {
            p.pos += p.vel * dt;
        }

        for (Projectile& p : projectiles.machineGun)
        {
            p.pos += p.vel * dt;
        }

        for (Projectile& p : projectiles.akimbo)
        {
            p.pos += p.vel * dt;
        }
        
        projectiles.rifle.erase(std::remove_if(projectiles.rifle.begin(), projectiles.rifle.end(), 
            [](Projectile& p)
            {
                p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_RIFLE, WORLD_REC);
                return p.destroy;
            }),
        projectiles.rifle.end());

        projectiles.shotgun.erase(std::remove_if(projectiles.shotgun.begin(), projectiles.shotgun.end(),
            [](Projectile& p)
            {
                p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_SHOTGUN, WORLD_REC);
                return p.destroy;
            }),
        projectiles.shotgun.end());

        projectiles.machineGun.erase(std::remove_if(projectiles.machineGun.begin(), projectiles.machineGun.end(),
            [](Projectile& p)
            {
                p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_MACHINE_GUN, WORLD_REC);
                return p.destroy;
            }),
        projectiles.machineGun.end());

        projectiles.akimbo.erase(std::remove_if(projectiles.akimbo.begin(), projectiles.akimbo.end(),
            [](Projectile& p)
            {
                p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_AKIMBO, WORLD_REC);
                return p.destroy;
            }),
        projectiles.akimbo.end());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        DrawCircleV(player.pos, RADIUS_PLAYER, BLUE);

        for (const Projectile& p : projectiles.rifle)
        {
            DrawCircleV(p.pos, RADIUS_RIFLE, RED);
        }

        for (const Projectile& p : projectiles.shotgun)
        {
            DrawCircleV(p.pos, RADIUS_SHOTGUN, GREEN);
        }

        for (const Projectile& p : projectiles.machineGun)
        {
            DrawCircleV(p.pos, RADIUS_MACHINE_GUN, BLUE);
        }

        for (const Projectile& p : projectiles.akimbo)
        {
            DrawCircleV(p.pos, RADIUS_AKIMBO, ORANGE);
        }

        for (const Ball& b : balls)
        {
            DrawCircleV(b.pos, RADIUS_BALL, b.color);
        }

        // Since everything is relative to the camera, just use world-space UI!
        //DrawText(TextFormat("mx: %f my: %f", mouse.x, mouse.y), player.pos.x, player.pos.y - 50.0f, 20, BLUE);
        EndMode2D();
        DrawFPS(10, 10);
        DrawText(TextFormat("Bullet count: %i", projectiles.rifle.size()), 10, 30, 20, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
