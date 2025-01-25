#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "Colors.h"
#include "Bullet.h"

constexpr float SCREEN_SIZE = 800.0f;

constexpr float RADIUS_BALL = 10.0f;
constexpr float RADIUS_PLAYER = 25.0f;

constexpr float WORLD_MIN = -2500.0f;
constexpr float WORLD_MAX =  2500.0f;

constexpr Rectangle WORLD_REC{ WORLD_MIN, WORLD_MIN, WORLD_MAX * 2.0f, WORLD_MAX * 2.0f };

//struct Rigidbody
//{
//    Vector2 pos = Vector2Zeros;
//    Vector2 vel = Vector2Zeros;
//    Vector2 acc = Vector2Zeros;
//
//    Vector2 dir = Vector2UnitX; // right
//    float angularSpeed = 0.0f;  // radians
//};

RMAPI inline float Random(float min, float max)
{
    return min + (rand() / ((float)RAND_MAX / (max - min)));
}

RMAPI float Sign(float value)
{
    float result = (value < 0.0f) ? -1.0f : 1.0f;

    return result;
}

RMAPI float Vector2Cross(Vector2 v1, Vector2 v2)
{
    float result = v1.x * v2.y - v1.y * v2.x;

    return result;
}

RMAPI float Vector2UnsignedAngle(Vector2 start, Vector2 end)
{
    float result = 0.0f;

    float dot = start.x * end.x + start.y * end.y;      // Dot product

    float dotClamp = (dot < -1.0f) ? -1.0f : dot;    // Clamp
    if (dotClamp > 1.0f) dotClamp = 1.0f;

    result = acosf(dotClamp);

    return result;
}

RMAPI Vector2 RotateTowards(Vector2 from, Vector2 to, float maxRadians)
{
    float deltaRadians = Vector2UnsignedAngle(from, to);
    return Vector2Rotate(from, fminf(deltaRadians, maxRadians) * Sign(Vector2Cross(from, to)));
}

// Player needs custom update. Maybe use this for AI? Remember to make whatever I need on a case-by-case basis, don't force all entities to follow common physics logic
//void Update(Rigidbody& rb, float dt)
//{
//    rb.vel = rb.vel + rb.acc * dt;
//    rb.pos = rb.pos + rb.vel * dt;
//    rb.dir = RotateTowards(rb.dir, Vector2Normalize(rb.vel), rb.angularSpeed * dt);
//}

inline Vector2 Seek(Vector2 target, Vector2 seekerPosition, Vector2 seekerVelocity, float speed)
{
    // From seeker to target with a magnitude (strength) of speed
    Vector2 desiredVelocity = Vector2Normalize(target - seekerPosition) * speed;

    // Apply difference as an acceleration
    return desiredVelocity - seekerVelocity;
}

struct Player
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    float moveSpeed = 0.0f;

    // Only support linear motiton. Player aims at the cursor, but doesn't rotate!
    //float turnSpeed = 0.0f;
    //float rotation = 0.0f;

    // Just change velocity based on some value, don't store acceleration cause it'll be confusing whether its supposed to be reset every frame
    //Vector2 acc = Vector2Zeros;
};

struct Projectile
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    bool destroy = false;
};

struct Projectiles
{
    std::vector<Projectile> rifle;
    std::vector<Projectile> shotgun;
    std::vector<Projectile> grenade;
};

// Test object
struct Ball
{
    Vector2 pos = Vector2Zeros;
    Color color = BLACK;
};

// TODO -- Randomply spawn circles & render them to test camera logic
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

    Camera2D camera;
    camera.target = player.pos;
    camera.offset = Vector2Ones * SCREEN_SIZE * 0.5f;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Projectiles projectiles;
    projectiles.rifle.reserve(128);
    projectiles.shotgun.reserve(128);
    projectiles.grenade.reserve(128);

    float bulletCooldownCurrent = 0.0f;
    const float bulletCooldownTotal = 0.5f;

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

        bulletCooldownCurrent += dt;
        if (bulletCooldownCurrent >= bulletCooldownTotal)
        {
            if (IsKeyDown(KEY_SPACE))
            {
                bulletCooldownCurrent = 0.0f;

                Vector2 mouseDirection = Vector2Normalize(mouse - player.pos);

                Projectile pRifle;
                pRifle.pos = player.pos + mouseDirection * (RADIUS_PLAYER + RADIUS_RIFLE);
                pRifle.vel = mouseDirection * 1000.0f;

                projectiles.rifle.push_back(pRifle);
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
        
        projectiles.rifle.erase(std::remove_if(projectiles.rifle.begin(), projectiles.rifle.end(), 
            [](Projectile& p)
            {
                // Removed if true
                p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_RIFLE, WORLD_REC);
                return p.destroy;
            }),
        projectiles.rifle.end());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        DrawCircleV(player.pos, RADIUS_PLAYER, BLUE);

        for (const Projectile& p : projectiles.rifle)
        {
            DrawCircleV(p.pos, RADIUS_RIFLE, RED);
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
