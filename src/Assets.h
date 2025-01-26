#pragma once
#include <raylib.h>
#include "Assets.h"

extern Texture2D gTexBubble;

void LoadAssets();
void UnloadAssets();

void TexRecFromCircle(Texture2D tex, Vector2 pos, float radius, Rectangle* src, Rectangle* dst);
