#include <raylib.h>
#include "Colors.h"
#include "Weapons.h"
#include "Steering.h"
#include "Timer.h"

constexpr Rectangle WORLD_REC{ WORLD_MIN, WORLD_MIN, WORLD_MAX * 2.0f, WORLD_MAX * 2.0f };

struct Player
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    float moveSpeed = 0.0f;

    WeaponType weaponType = RIFLE;
    Timer shootTimer;
};

// Test object
struct Ball
{
    Vector2 pos = Vector2Zeros;
    Color color = BLACK;
};

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubblio");
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
    projectiles.grenade.reserve(128);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

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

        //player.vel += player.acc * dt;
        player.pos += player.vel * dt;

        camera.target = player.pos;

        Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);

        Tick(player.shootTimer, dt);
        if (Expired(player.shootTimer))
        {
            if (IsKeyDown(KEY_SPACE))
            {
                Reset(player.shootTimer);

                switch (player.weaponType)
                {
                case RIFLE:
                {
                    Projectile rifle = ShootRifle(player.pos, RADIUS_PLAYER, Vector2Normalize(mouse - player.pos));
                    projectiles.rifle.push_back(rifle);
                }
                break;

                case SHOTGUN:
                {
                    std::array<Projectile, 3> shotgun = ShootShotgun(player.pos, RADIUS_PLAYER, Vector2Normalize(mouse - player.pos));
                    for (Projectile& p : shotgun)
                        projectiles.shotgun.push_back(p);
                }
                break;

                case MACHINE_GUN:
                    break;
                }
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
