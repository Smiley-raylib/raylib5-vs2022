#pragma once
#include "Weapons.h"
#include "Timer.h"

struct Player
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    float moveSpeed = 0.0f;

    int weaponType = RIFLE;
    Timer shootTimer;
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

        case ROCKET:
        {
            std::array<Projectile, 5> rocket = ShootRocket(position, radius, direction);
            for (Projectile& p : rocket)
                projectiles.rocket.push_back(p);
        }
        break;
    }
}
