#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "Colors.h"

constexpr float SCREEN_SIZE = 800.0f;

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
    Vector2 acc = Vector2Zeros;
    float moveSpeed = 0.0f;

    // Only support linear motiton. Player aims at the cursor, but doesn't rotate!
    //float turnSpeed = 0.0f;
    //float rotation = 0.0f;
};

struct Projectile
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    Vector2 acc = Vector2Zeros;
};

// TODO -- Randomply spawn circles & render them to test camera logic
int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubblio");
    SetTargetFPS(60);

    std::vector<Vector2> positions;
    std::vector<Color> colors;
    positions.resize(128);
    colors.resize(128);
    for (int i = 0; i < positions.size(); i++)
    {
        positions[i].x = Random(-2000.0f, 2000.0f);
        positions[i].y = Random(-2000.0f, 2000.0f);
        colors[i] = ballColors[rand() % ballColors.size()];
    }

    Player player;
    player.pos = Vector2Ones * SCREEN_SIZE * 0.5f;
    player.moveSpeed = SCREEN_SIZE * 0.5f;

    Camera2D camera;
    camera.target = player.pos;
    camera.offset = Vector2Ones * SCREEN_SIZE * 0.5f;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Projectile bullet;
    bullet.pos = player.pos;
    bullet.vel = Vector2Zeros;

    //std::vector<Vector2> projectiles;
    //projectiles.reserve(1024);

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

        player.vel += player.acc * dt;
        player.pos += player.vel * dt;

        camera.target = player.pos;

        Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);

        if (IsKeyPressed(KEY_SPACE))
        {
            Vector2 mouseDirection = Vector2Normalize(mouse - player.pos);
            bullet.pos = player.pos;
            bullet.vel = mouseDirection * 250.0f;
        }

        bullet.pos += bullet.vel * dt;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        DrawCircleV(player.pos, 50, BLUE);
        DrawCircleV(bullet.pos, 25, RED);
        for (int i = 0; i < positions.size(); i++)
        {
            Vector2 pos = positions[i];
            Color col = colors[i];
            DrawCircleV(pos, 10.0f, col);
        }

        // Since everything is relative to the camera, just use world-space UI!
        //DrawText(TextFormat("mx: %f my: %f", mouse.x, mouse.y), player.pos.x, player.pos.y - 50.0f, 20, BLUE);
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
