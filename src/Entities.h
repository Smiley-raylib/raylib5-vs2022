#pragma once
#include "Weapons.h"
#include "Timer.h"
#include "Steering.h"

struct Player
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    float moveSpeed = 0.0f;

    int weaponType = RIFLE;
    Timer shootTimer;

    float health = 100.0f;
};

struct Enemy
{
    Vector2 pos = Vector2Zeros;
    Vector2 vel = Vector2Zeros;
    Vector2 acc = Vector2Zeros;

    int weaponType = RIFLE;
    Timer shootTimer;

    float health = 100.0f;
};

using Enemies = std::vector<Enemy>;

// TODO - give enemies specific behaviour when I'm not high on magic mushrooms
//struct Enemies
//{
//    std::vector<Enemy> rifle;
//    std::vector<Enemy> shotgun;
//    std::vector<Enemy> machine;
//    std::vector<Enemy> akimbo;
//    std::vector<Enemy> rocket;
//};

void Shoot(Vector2 position, Vector2 direction, float radius, Projectiles& projectiles, int type, int team)
{
    switch (type)
    {
        case RIFLE:
        {
            Projectile rifle = ShootRifle(position, radius, direction);
            rifle.team = team;
            projectiles.rifle.push_back(rifle);
        }
        break;

        case SHOTGUN:
        {
            std::array<Projectile, 3> shotgun = ShootShotgun(position, radius, direction);
            for (Projectile& p : shotgun)
            {
                p.team = team;
                projectiles.shotgun.push_back(p);
            }
        }
        break;

        case MACHINE_GUN:
        {
            Projectile machine = ShootMachineGun(position, radius, direction);
            machine.team = team;
            projectiles.machineGun.push_back(machine);
        }
        break;

        case AKIMBO:
        {
            std::array<Projectile, 2> akimbo = ShootAkimbo(position, radius, direction);
            for (Projectile& p : akimbo)
            {
                p.team = team;
                projectiles.akimbo.push_back(p);
            }
        }
        break;

        case ROCKET:
        {
            std::array<Projectile, 5> rocket = ShootRocket(position, radius, direction);
            for (Projectile& p : rocket)
            {
                p.team = team;
                projectiles.rocket.push_back(p);
            }
        }
        break;
    }
}

void UpdateEnemies(const Player& player, Enemies& enemies, Projectiles& projectiles, float dt)
{
    for (Enemy& enemy : enemies)
    {
        enemy.acc = Seek(player.pos, enemy.pos, enemy.vel, 500.0f);
        enemy.vel += enemy.acc * dt;
        enemy.pos += enemy.vel * dt;
        enemy.pos = Vector2Clamp(enemy.pos, Vector2Ones * WORLD_MIN, Vector2Ones * WORLD_MAX);

        Tick(enemy.shootTimer, dt);
        if (Expired(enemy.shootTimer))
        {
            Vector2 playerDirection = Vector2Normalize(player.pos - enemy.pos);
            Reset(enemy.shootTimer);
            Shoot(enemy.pos, playerDirection, RADIUS_ENEMY, projectiles, enemy.weaponType, ENEMY);
        }
    }
}
