#pragma once
#include <raylib.h>
#include <array>

constexpr float SCREEN_SIZE = 800.0f;
constexpr float WORLD_MIN = -2500.0f;
constexpr float WORLD_MAX = 2500.0f;

constexpr float RADIUS_PLAYER = 25.0f;

constexpr float RADIUS_RIFLE = 10.0f;
constexpr float RADIUS_SHOTGUN = 12.5f;
constexpr float RADIUS_MACHINE_GUN = 5.0f;
constexpr float RADIUS_AKIMBO = 5.0f;
constexpr float RADIUS_ROCKET = 20.0f;

constexpr float COOLDOWN_RIFLE = 0.5f;
constexpr float COOLDOWN_SHOTGUN = 0.75f;
constexpr float COOLDOWN_MACHINE_GUN = 0.2f;
constexpr float COOLDOWN_AKIMBO = 0.3f;
constexpr float COOLDOWN_ROCKET = 1.0f;

constexpr float SPEED_RIFLE = 1000.0f;
constexpr float SPEED_SHOTGUN = 500.0f;
constexpr float SPEED_MACHINE_GUN = 750.0f;
constexpr float SPEED_AKIMBO = 600.0f;
constexpr float SPEED_ROCKET = 450.0f;

constexpr float LIFE_RIFLE = 1.0f;
constexpr float LIFE_SHOTGUN = 0.75f;
constexpr float LIFE_MACHINE_GUN = 0.5f;
constexpr float LIFE_AKIMBO = 0.6f;
constexpr float LIFE_ROCKET = 2.0f;

constexpr Color COLOR_RIFLE = RED;
constexpr Color COLOR_SHOTGUN = GREEN;
constexpr Color COLOR_MACHINE_GUN = BLUE;
constexpr Color COLOR_AKIMBO = ORANGE;
constexpr Color COLOR_ROCKET = MAGENTA;

constexpr std::array<Color, 21> COLORS =
{
	LIGHTGRAY,
	GRAY,
	DARKGRAY,
	YELLOW,
	GOLD,
	ORANGE,
	PINK,
	RED,
	MAROON,
	GREEN,
	LIME,
	DARKGREEN,
	SKYBLUE,
	BLUE,
	DARKBLUE,
	PURPLE,
	VIOLET,
	DARKPURPLE,
	BEIGE,
	BROWN,
	LIGHTGRAY
};
