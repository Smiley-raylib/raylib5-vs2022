#include <raylib.h>
#include <raymath.h>

constexpr float SCREEN_SIZE = 800.0f;

struct Rigidbody
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    Vector2 acc = Vector2Zeros;

    Vector2 dir = Vector2UnitX; // right
    float angularSpeed = 0.0f;  // radians
};

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

// Kinematic physics update
void Update(Rigidbody& rb, float dt)
{
    rb.vel = rb.vel + rb.acc * dt;
    rb.pos = rb.pos + rb.vel * dt + rb.acc * dt * dt * 0.5f;
    rb.dir = RotateTowards(rb.dir, Vector2Normalize(rb.vel), rb.angularSpeed * dt);
}

inline Vector2 Seek(Vector2 target, Vector2 seekerPosition, Vector2 seekerVelocity, float speed)
{
    // From seeker to target with a magnitude (strength) of speed
    Vector2 desiredVelocity = Vector2Normalize(target - seekerPosition) * speed;

    // Apply difference as an acceleration
    return desiredVelocity - seekerVelocity;
}

int main()
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Bubblio");
    SetTargetFPS(60);

    Rigidbody rbPlayer;
    rbPlayer.pos = Vector2Ones * SCREEN_SIZE * 0.5f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        Vector2 mouseDirection = Vector2Normalize(GetMousePosition() - rbPlayer.pos);
        
        // Agario uses linear seek
        //rbPlayer.pos += mouseDirection * 400.0f * dt;

        // Should I use force-seek to make Bubblio different!?
        rbPlayer.acc = Seek(GetMousePosition(), rbPlayer.pos, rbPlayer.vel, 1000.0f);
        Update(rbPlayer, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(rbPlayer.pos, 50, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
