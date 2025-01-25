#pragma once
#include "Projectiles.h"
#include "Map.h"

constexpr Rectangle WORLD_REC{ WORLD_MIN, WORLD_MIN, WORLD_MAX * 2.0f, WORLD_MAX * 2.0f };

void UpdateProjectiles(Projectiles& projectiles, const Map& map,  float dt)
{
    for (Projectile& p : projectiles.rifle)
    {
        p.pos += p.vel * dt;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_RIFLE, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_RIFLE, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.shotgun)
    {
        p.pos += p.vel * dt;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_SHOTGUN, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_SHOTGUN, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.machineGun)
    {
        p.pos += p.vel * dt;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_MACHINE_GUN, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_MACHINE_GUN, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.akimbo)
    {
        p.pos += p.vel * dt;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_AKIMBO, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_AKIMBO, WORLD_REC);
        }
    }
}

void PruneProjectiles(Projectiles& projectiles)
{
    projectiles.rifle.erase(std::remove_if(projectiles.rifle.begin(), projectiles.rifle.end(),
        [](Projectile& p) { return p.destroy; }), projectiles.rifle.end());

    projectiles.shotgun.erase(std::remove_if(projectiles.shotgun.begin(), projectiles.shotgun.end(),
        [](Projectile& p) { return p.destroy; }), projectiles.shotgun.end());

    projectiles.machineGun.erase(std::remove_if(projectiles.machineGun.begin(), projectiles.machineGun.end(),
        [](Projectile& p) { return p.destroy; }), projectiles.machineGun.end());

    projectiles.akimbo.erase(std::remove_if(projectiles.akimbo.begin(), projectiles.akimbo.end(),
        [](Projectile& p) { return p.destroy; }), projectiles.akimbo.end());
}
