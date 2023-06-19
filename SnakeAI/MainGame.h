#pragma once

#include <GEngine_v0.1/Game.h>
#include <DeepQLearning/Agent.h>
#include "Board.h"
#include <DeepQLearning/ActivationFuncs.h>
#include <memory>

class MainGame
{
public:
	MainGame(GEngine::Game& game);

	void Init();
	void Update();
	void Draw();

	Board& GetBoard() { return board; }

private:
	GEngine::Game& game;
	Board board;

	dqn::ReLU relu;
	dqn::Linear linear;

	std::vector<dqn::Agent::Action> actions;
	std::vector<dqn::LayerInfo> layerInfo;

	dqn::Agent agent;
	std::string name;
};