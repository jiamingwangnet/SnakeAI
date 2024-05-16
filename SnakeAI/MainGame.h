#pragma once

#include <GEngine_v0.1/Game.h>
#include <ReinforcementLearning/DQNAgent.h>
#include "Board.h"
#include <ReinforcementLearning/ActivationFuncs.h>
#include <memory>
#include "WeightsDisplayer.h"
#include <ReinforcementLearning/PPOAgent.h>

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

	std::shared_ptr<net::ReLU> relu = std::make_shared<net::ReLU>();
	std::shared_ptr<net::Linear> linear = std::make_shared<net::Linear>();

	std::vector<net::Agent::Action> actions;

	std::vector<net::LayerInfo> layerInfo;

#ifdef PPO
	ppo::PPOAgent agent;
#else
	dqn::DQNAgent agent;
#endif
	std::string name;
};