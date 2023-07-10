#pragma once

#include <GEngine_v0.1/Game.h>
#include <ReinforcementLearning/Agent.h>
#include "Board.h"
#include <ReinforcementLearning/ActivationFuncs.h>
#include <memory>
#include "WeightsDisplayer.h"

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
	WeightsDisplayer display;

	net::ReLU relu;
	net::Linear linear;

	std::vector<dqn::Agent::Action> actions;
	std::vector<net::LayerInfo> layerInfo;

	dqn::Agent agent;
	std::string name;
};