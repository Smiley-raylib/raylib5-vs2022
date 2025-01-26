#pragma once
#include <raylib.h>
#include "MathExt.h"
#include <array>
#include <vector>
#include "Assets.h"
// *Must include raylib before raymath cause raylib defines VectorN*

enum WeaponType : int
{
	RIFLE,
	SHOTGUN,
	MACHINE_GUN,
	AKIMBO,
	ROCKET,
	WEAPON_COUNT
};

struct Projectile
{
	Vector2 pos = Vector2Zeros;
	Vector2 vel = Vector2Zeros;
	float time = 0.0f;
	bool destroy = false;

	Vector2 launchPos = Vector2Zeros;
	Vector2 launchDir = Vector2Zeros;
	float phase = 0.0f;
};

struct Projectiles
{
	std::vector<Projectile> rifle;
	std::vector<Projectile> shotgun;
	std::vector<Projectile> machineGun;
	std::vector<Projectile> akimbo;
	std::vector<Projectile> rocket;
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

inline std::array<Projectile, 2> ShootAkimbo(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	std::array< Projectile, 2> p;
	p[0].pos = p[1].pos = shooterPosition + shootDirection * (shooterRadius + RADIUS_AKIMBO);
	p[1].pos += shootDirection * RADIUS_AKIMBO * 4.0f;
	p[0].vel = p[1].vel = shootDirection * SPEED_AKIMBO;
	return p;
}

inline std::array<Projectile, 5> ShootRocket(Vector2 shooterPosition, float shooterRadius, Vector2 shootDirection)
{
	std::array<Projectile, 5> p;

	const float spread = PI / 5.0f;
	float angle = -spread * 2.0f;
	float r = 100.0f;
	for (int i = 0; i < 5; i++)
	{
		Vector2 dir = Vector2Rotate(shootDirection * -1.0f, angle);
		p[i].pos = shooterPosition + dir * r + shootDirection * r;

		dir *= -1.0f;
		p[i].vel = dir * SPEED_ROCKET;

		p[i].launchPos = p[i].pos;
		p[i].launchDir = dir;

		p[i].phase = spread * i;
		angle += spread;
	}

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

	case ROCKET:
		return COOLDOWN_ROCKET;
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

void DrawRocket(Vector2 pos)
{
	DrawTexCircle(gTexBubble, pos, RADIUS_ROCKET, COLOR_ROCKET);
}
