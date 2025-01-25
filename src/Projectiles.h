#pragma once
#include "MathExt.h"
#include <vector>

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
