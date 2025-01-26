#include "Assets.h"

// 32x32
Texture2D gTexBubble;

void LoadAssets()
{
	gTexBubble = LoadTexture("./assets/bubble.png");
}

void UnloadAssets()
{
	UnloadTexture(gTexBubble);
}

void TexRecFromCircle(Texture2D tex, Vector2 pos, float radius, Rectangle* src, Rectangle* dst)
{
	src->x = 0;
	src->y = 0;
	src->width = tex.width;
	src->height = tex.height;

	dst->x = pos.x - radius;
	dst->y = pos.y - radius;
	dst->width = radius * 2.0f;
	dst->height = radius * 2.0f;
}
