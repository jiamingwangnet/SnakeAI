#include "WeightsDisplayer.h"
#include <GEngine_v0.1/Game.h>

WeightsDisplayer::WeightsDisplayer(GEngine::Game& game, net::Agent& agent, float x, float y, float tileSize)
	: agent{agent}, x{x}, y{y}, tileSize{tileSize}, game{game}
{}

void WeightsDisplayer::Init()
{
	net::NeuralNet& net = agent.GetMainNet();

	size_t accumX = 0;

	for ( size_t lindex = 1; lindex < net.GetLayers().size(); lindex++)
	{
		const std::unique_ptr<net::Layer>& layer = net.GetLayers()[lindex];

		size_t input = net.GetLayers()[lindex-1]->GetNodes();
		size_t weightY = layer->GetWeights().GetColumns();
		size_t weightX = layer->GetWeights().GetRows();
		size_t biases = layer->GetBiases().GetColumns();
		
		std::vector<Tile*> inputVec;
		for (size_t i = 0; i < input; i++)
		{
			inputVec.push_back(game.CreateGameObject<Tile>(GEngine::Vector2{ x + i * (tileSize + PADDING) + (tileSize + PADDING) * accumX, y }, GEngine::Vector2{tileSize, tileSize}, GEngine::Color{100, 100, 100, 255}));
		}
		tiles.push_back(inputVec);

		std::vector<Tile*> weightsVec;
		for (size_t i = 0; i < weightY; i++)
		{
			for (size_t j = 0; j < weightX; j++)
			{
				weightsVec.push_back(game.CreateGameObject<Tile>(GEngine::Vector2{ x + j * (tileSize + PADDING) + (tileSize + PADDING) * accumX, y + i * (tileSize + PADDING) + 8.0f }, GEngine::Vector2{ tileSize, tileSize }, GEngine::Color{ 100, 100, 100, 255 }));
			}
		}
		tiles.push_back(weightsVec);
		accumX += weightX;
		/*
		std::vector<Tile*> biasVec;
		for (size_t i = 0; i < biases; i++)
		{
			biasVec.push_back(game.CreateGameObject<Tile>(GEngine::Vector2{ x + (tileSize + PADDING) * accumX, y + i * (tileSize + PADDING) }, GEngine::Vector2{ tileSize, tileSize }, GEngine::Color{ 100, 100, 100, 255 }));
		}
		tiles.push_back(biasVec);
		accumX += 1;*/

		accumX += 3;
	}


	size_t input = net.GetLayers().back()->GetNodes();
	std::vector<Tile*> inputVec;
	for (size_t i = 0; i < input; i++)
	{
		inputVec.push_back(game.CreateGameObject<Tile>(GEngine::Vector2{ x + i * (tileSize + PADDING) + (tileSize + PADDING) * accumX, y }, GEngine::Vector2{ tileSize, tileSize }, GEngine::Color{ 100, 100, 100, 255 }));
	}
	tiles.push_back(inputVec);
}

void WeightsDisplayer::Update()
{
	net::NeuralNet& net = agent.GetMainNet();

	int l = 0;
	for (size_t lindex = 1; lindex < net.GetLayers().size(); lindex++)
	{
		const std::unique_ptr<net::Layer>& layer = net.GetLayers()[lindex];

		const net::DMatrix& inputs = net.GetLayers()[lindex - 1]->GetOutputs();
		std::vector<Tile*>& tileVec = tiles[l];
		
		int i = 0;
		for (const double& in : inputs)
		{
			double input = in;
			if (in > iclmpMax) input = iclmpMax;
			if (in < iclmpMin) input = iclmpMin;
			GEngine::Color color;
			
			if (input < 0)
			{
				// red
				color = GEngine::Color{ (GLubyte)(255.0 * (input / iclmpMin)), 0, 0, 255 };
			}
			else
			{
				// green
				color = GEngine::Color{ 0, (GLubyte)(255.0 * (input / iclmpMax)), 0, 255 };
			}

			tileVec[i]->SetColor(color);

			i++;
		}

		const net::DMatrix& weights = layer->GetWeights();
		std::vector<Tile*>& wtileVec = tiles[++l];

		i = 0;
		for (const double& w : weights)
		{
			double weight = w;
			if (w > clmpMax) weight = clmpMax;
			if (w < clmpMin) weight = clmpMin;
			GEngine::Color color;

			if (weight < 0)
			{
				// red
				color = GEngine::Color{ (GLubyte)(255.0 * (weight / clmpMin)), 0, 0, 255 };
			}
			else
			{
				// green
				color = GEngine::Color{ 0, (GLubyte)(255.0 * (weight / clmpMax)), 0, 255 };
			}

			wtileVec[i]->SetColor(color);

			i++;
		}

		l++;
	}

	const net::DMatrix& inputs = net.GetLayers().back()->GetOutputs();
	std::vector<Tile*>& tileVec = tiles.back();

	int i = 0;
	for (const double& in : inputs)
	{
		double input = in;
		if (in > oclmpMax) input = oclmpMax;
		if (in < oclmpMin) input = oclmpMin;
		GEngine::Color color;

		if (input < 0)
		{
			// red
			color = GEngine::Color{ (GLubyte)(255.0 * (input / oclmpMin)), 0, 0, 255 };
		}
		else
		{
			// green
			color = GEngine::Color{ 0, (GLubyte)(255.0 * (input / oclmpMax)), 0, 255 };
		}

		tileVec[i]->SetColor(color);

		i++;
	}
}
