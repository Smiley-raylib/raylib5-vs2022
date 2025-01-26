#pragma once
#include <raylib.h>
#include "MathExt.h"
#include <array>
#include <vector>
#include "Assets.h"
// *Must include raylib before raymath cause raylib defines VectorN*

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
	std::vector<Projectile> machineGun;
	std::vector<Projectile> akimbo;
};

enum WeaponType : int
{
	RIFLE,
	SHOTGUN,
	MACHINE_GUN,
	AKIMBO,
	ROCKET_LAUNCHER,
	WEAPON_COUNT
};

inline Projectile ShootRifle(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	Projectile p;
	p.pos = shooterPosition + shootDirection * (shooterRadius + RADIUS_RIFLE);
	p.vel = shootDirection * SPEED_RIFLE;
	return p;
}

inline std::array<Projectile, 3> ShootShotgun(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
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

inline Projectile ShootMachineGun(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	// 50% chance of not shooting exactly in shoot_direction
	bool miss = (rand() % 4) < 2;
	shootDirection = miss ? Vector2Rotate(shootDirection, Random(-10.0f, 10.0f) * DEG2RAD) : shootDirection;

	Projectile p;
	p.pos = shooterPosition + shootDirection * (shooterRadius + RADIUS_MACHINE_GUN);
	p.vel = shootDirection * SPEED_MACHINE_GUN;
	return p;
}

inline std::array< Projectile, 2> ShootAkimbo(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	std::array< Projectile, 2> p;
	p[0].pos = p[1].pos = shooterPosition + shootDirection * (shooterRadius + RADIUS_AKIMBO);
	p[1].pos += shootDirection * RADIUS_AKIMBO * 4.0f;
	p[0].vel = p[1].vel = shootDirection * SPEED_AKIMBO;
	return p;
}

inline float WeaponCooldown(int type)
{
	switch (type)
	{
	case RIFLE:
		return COOLDOWN_RIFLE;

	case SHOTGUN:
		return COOLDOWN_SHOTGUN;

	case MACHINE_GUN:
		return COOLDOWN_MACHINE_GUN;

	case AKIMBO:
		return COOLDOWN_AKIMBO;
	}
}

void DrawTexCircle(Texture2D tex, Vector2 pos, float radius, Color tint = WHITE)
{
	Rectangle src, dst;
	TexRecFromCircle(tex, pos, radius, &src, &dst);
	DrawTexturePro(tex, src, dst, Vector2Zeros, 0.0f, tint);
}

void DrawRifle(Vector2 pos)
{
	DrawTexCircle(gTexBubble, pos, RADIUS_RIFLE, COLOR_RIFLE);
}

void DrawShotgun(Vector2 pos)
{
	DrawTexCircle(gTexBubble, pos, RADIUS_SHOTGUN, COLOR_SHOTGUN);
}


void DrawMachineGun(Vector2 pos)
{
	DrawTexCircle(gTexBubble, pos, RADIUS_MACHINE_GUN, COLOR_MACHINE_GUN);
}

void DrawAkimbo(Vector2 pos)
{
	DrawTexCircle(gTexBubble, pos, RADIUS_AKIMBO, COLOR_AKIMBO);
}