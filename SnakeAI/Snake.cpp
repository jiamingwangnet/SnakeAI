#include "Snake.h"

Snake::Snake(int length, int x, int y)
	: body( length ), heading{ EAST }
{
	for (Section& section : body)
	{
		section.y = y;
		section.x = x + --length;
	}
}

bool Snake::Collide(int width, int height, const std::vector<Tile*>& tiles, const GEngine::Color& snakeColor)
{
	return
		body[0].x + heading[0] >= width || body[0].y + heading[1] >= height || body[0].x + heading[0] < 0 || body[0].y + heading[1] < 0 ||
		tiles[(body[0].y + heading[1]) * width + (body[0].x + heading[0])]->GetColor() == snakeColor;
}

void Snake::Update()
{
	Section last = body[0];

	// move the head
	body[0].x += heading[0];
	body[0].y += heading[1];

	//if (body[0].x >= width || body[0].y >= height || body[0].x < 0 || body[0].y < 0) return;

	for (auto it = body.begin() + 1; it != body.end(); ++it)
	{
		Section tmp = last;
		last = *it;
		it->x = tmp.x;
		it->y = tmp.y;
	}
}
