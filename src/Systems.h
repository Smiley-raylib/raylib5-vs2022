#pragma once
#include "Map.h"

constexpr Rectangle WORLD_REC{ WORLD_MIN, WORLD_MIN, WORLD_MAX * 2.0f, WORLD_MAX * 2.0f };

void UpdateProjectiles(Projectiles& projectiles, const Map& map,  float dt)
{
    for (Projectile& p : projectiles.rifle)
    {
        p.pos += p.vel * dt;
        p.time += dt;
        p.destroy |= p.time >= LIFE_RIFLE;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_RIFLE, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_RIFLE, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.shotgun)
    {
        p.pos += p.vel * dt;
        p.time += dt;
        p.destroy |= p.time >= LIFE_SHOTGUN;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_SHOTGUN, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_SHOTGUN, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.machineGun)
    {
        p.pos += p.vel * dt;
        p.time += dt;
        p.destroy |= p.time >= LIFE_MACHINE_GUN;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_MACHINE_GUN, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_MACHINE_GUN, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.akimbo)
    {
        p.pos += p.vel * dt;
        p.time += dt;
        p.destroy |= p.time >= LIFE_AKIMBO;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_AKIMBO, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_AKIMBO, WORLD_REC);
        }
    }

    for (Projectile& p : projectiles.rocket)
    {
        p.pos += p.vel * dt;
        p.time += dt;
        p.destroy |= p.time >= LIFE_ROCKET;

        float t = p.time / LIFE_ROCKET; // lerp from 0 to 1
        t = 1.0f - t;                   // lerp from 1 to 0
        t = powf(t, 2.0f);              // quadratic easing

        Vector2 perp{ -p.launchDir.y, p.launchDir.x };
        p.pos += perp * cosf(p.time * 8.0f + p.phase) * 25.0f * t;

        for (const Obstacle& o : map.obstacles)
        {
            p.destroy |= CheckCollisionCircles(p.pos, RADIUS_ROCKET, o.pos, o.radius);
            p.destroy |= !CheckCollisionCircleRec(p.pos, RADIUS_ROCKET, WORLD_REC);
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

    projectiles.rocket.erase(std::remove_if(projectiles.rocket.begin(), projectiles.rocket.end(),
        [](Projectile& p) { return p.destroy; }), projectiles.rocket.end());
}

void DrawProjectiles(const Projectiles& projectiles)
{
    for (const Projectile& p : projectiles.rifle)
    {
        DrawRifle(p.pos);
    }

    for (const Projectile& p : projectiles.shotgun)
    {
        DrawShotgun(p.pos);
    }

    for (const Projectile& p : projectiles.machineGun)
    {
        DrawMachineGun(p.pos);
    }

    for (const Projectile& p : projectiles.akimbo)
    {
        DrawAkimbo(p.pos);
    }

    for (const Projectile& p : projectiles.rocket)
    {
        DrawRocket(p.pos);
    }
}
