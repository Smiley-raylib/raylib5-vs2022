#pragma once
#include "MathExt.h"

inline Vector2 Seek(Vector2 target, Vector2 seekerPosition, Vector2 seekerVelocity, float speed)
{
    return (Vector2Normalize(target - seekerPosition) * speed) - seekerVelocity;
}
