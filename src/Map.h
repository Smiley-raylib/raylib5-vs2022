#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Obstacle
{
	Vector2 pos = Vector2Zeros;
	float radius = 0.0f;
	Color color = BLACK;
};

struct Map
{
	std::vector<Obstacle> obstacles;
};
