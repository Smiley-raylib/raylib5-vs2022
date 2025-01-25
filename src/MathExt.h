#pragma once
#include <cstdlib>
#include <raymath.h>

RMAPI float Random(float min, float max)
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

//struct Rigidbody
//{
//    Vector2 pos = Vector2Zeros;
//    Vector2 vel = Vector2Zeros;
//    Vector2 acc = Vector2Zeros;
//
//    Vector2 dir = Vector2UnitX; // right
//    float angularSpeed = 0.0f;  // radians
//};

// Player needs custom update. Maybe use this for AI? Remember to make whatever I need on a case-by-case basis, don't force all entities to follow common physics logic
//void Update(Rigidbody& rb, float dt)
//{
//    rb.vel = rb.vel + rb.acc * dt;
//    rb.pos = rb.pos + rb.vel * dt;
//    rb.dir = RotateTowards(rb.dir, Vector2Normalize(rb.vel), rb.angularSpeed * dt);
//}
