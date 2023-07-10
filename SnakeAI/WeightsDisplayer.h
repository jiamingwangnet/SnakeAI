#pragma once

#include <DeepQLearning/Agent.h>
#include "Tile.h"

class WeightsDisplayer
{
public:
	WeightsDisplayer(GEngine::Game& game, const dqn::Agent& agent, float x, float y, float tileSize);

	void Init();
	void Update();
private:
	const dqn::Agent& agent;
	static constexpr float PADDING = 0.0f;
	static constexpr double clmpMax = 1.0;
	static constexpr double clmpMin = -1.0;

	static constexpr double iclmpMax =  2.0;
	static constexpr double iclmpMin = -2.0;

	static constexpr double oclmpMax = 10.0;
	static constexpr double oclmpMin = -10.0;

	float x;
	float y;
	float tileSize;

	std::vector<std::vector<Tile*>> tiles;
	GEngine::Game& game;
};