#pragma once

#include <vector>
#include "Tile.h"

class Snake
{
public:
	static constexpr int NORTH [2] = {  0,  1 };
	static constexpr int EAST  [2] = {  1,  0 };
	static constexpr int SOUTH [2] = {  0, -1 };
	static constexpr int WEST  [2] = { -1,  0 };

	struct Section
	{
		int x;
		int y;
	};

public:
	Snake(int length, int x, int y);
	void Update();
	bool Collide(int width, int height, const std::vector<Tile*>& tiles, const GEngine::Color& snakeColor);
	std::vector<Section>& GetBody() { return body; }
	const int* GetHeading() { return heading; }
	void SetHeading(const int* heading) { 
		if (heading == NORTH && this->heading == SOUTH) return;
		if (heading == SOUTH && this->heading == NORTH) return;
		if (heading == EAST && this->heading == WEST) return;
		if (heading == WEST && this->heading == EAST) return;

		this->heading = heading;
	}

private:
	std::vector<Section> body;
	const int* heading;
};

