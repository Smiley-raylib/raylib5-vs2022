#pragma once
#include "Constants.h"
#include "Projectiles.h"
#include <array>

enum WeaponType
{
	RIFLE,
	SHOTGUN,
	MACHINE_GUN,
	WEAPON_COUNT
};

Projectile ShootRifle(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	Projectile p;
	p.pos = shooterPosition + shootDirection * (shooterRadius + RADIUS_RIFLE);
	p.vel = shootDirection * SPEED_RIFLE;
	return p;
}

std::array<Projectile, 3> ShootShotgun(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	std::array<Projectile, 3> p;

	Vector2 dirA = shootDirection;
	Vector2 dirB = Vector2Rotate(shootDirection, -30.0f * DEG2RAD);
	Vector2 dirC = Vector2Rotate(shootDirection, 30.0f * DEG2RAD);

	p[0].pos = shooterPosition + dirA * (shooterRadius + RADIUS_SHOTGUN);
	p[1].pos = shooterPosition + dirB * (shooterRadius + RADIUS_SHOTGUN);
	p[2].pos = shooterPosition + dirC * (shooterRadius + RADIUS_SHOTGUN);

	p[0].vel = dirA * SPEED_SHOTGUN;
	p[1].vel = dirB * SPEED_SHOTGUN;
	p[2].vel = dirC * SPEED_SHOTGUN;

	return p;
}
