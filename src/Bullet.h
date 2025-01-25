#pragma once
#include <raymath.h>

enum WeaponType
{
	RIFLE,
	SHOTGUN,
	MACHINE_GUN,
	WEAPON_COUNT
};

constexpr float RADIUS_RIFLE = 10.0f;
constexpr float RADIUS_SHOTGUN = 15.0f;
constexpr float RADIUS_MACHINE_GUN = 5.0f;

constexpr float COOLDOWN_RIFLE = 0.5f;
constexpr float COOLDOWN_SHOTGUN = 0.75f;
constexpr float COOLDOWN_MACHINE_GUN = 0.25f;
