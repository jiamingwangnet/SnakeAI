#include <GEngine_v0.1/Game.h>
#include "MainGame.h"
#include <iostream>
#include <DeepQLearning/Utility.h>
#include "Trainer.h"
#include <conio.h>



bool isSafe(int x, int y)
{
	return ((x / 2) + (x / 2) * (x / 2)) < y && y < (-6 * x * x + 10 * x * x * x);
}


void Test()
{
	using namespace dqn;

	typedef NeuralNet::DataPoint DataPoint;

	// {0, 1} unsafe
	// {1, 0} safe
	DataPoint unsafe{ {{4.28, 2.87},1,2},{}, { {0.0,1.0},1,2} };
	DataPoint safe{ {{2.45, 5.5},1,2},{}, { {1.0,0.0},1,2} };

	std::vector<DataPoint> data;
	for (int i = 0; i < 20000; i++)
	{
		DataPoint dp;

		int x = Util::Random<int>(std::uniform_int_distribution<int>{0, 10});
		int y = Util::Random<int>(std::uniform_int_distribution<int>{0, 10});

		dp.input = DMatrix{ {(double)x, (double)y}, 1, 2 };
		dp.expected = (isSafe(x, y) ? DMatrix{ {1.0, 0.0}, 1, 2 } : DMatrix{ {0.0, 1.0}, 1, 2 });

		data.push_back(dp);
	}

	Sigmoid sig;

	NeuralNet network{ {
		{2, 0, &sig},
		{3, 2, &sig},
		{2, 3, &sig}
		} };

	NeuralNet net2{ network };

	Trainer trainer{ data, 100, 0.8f };

	for (size_t index = 0, i = 0;; index++, i++)
	{
		trainer.Train(network, 0.05, index);
		net2 = network;
		safe.predicted = network.CalculateOutputs(safe.input);
		unsafe.predicted = network.CalculateOutputs(unsafe.input);

		std::cout << "---------------------------------------------------\n";
		std::cout << "epoch: " << i << '\n';
		std::cout << "0 | predicted: safe: " << (safe.predicted[0] * 100.0) << "% unsafe: " << (safe.predicted[1] * 100.0) << "% expected: safe: " << (safe.expected[0] * 100.0) << "% unsafe: " << (safe.expected[1] * 100.0) << '%' << '\n';
		std::cout << "1 | predicted: safe: " << (unsafe.predicted[0] * 100.0) << "% unsafe: " << (unsafe.predicted[1] * 100.0) << "% expected: safe: " << (unsafe.expected[0] * 100.0) << "% unsafe: " << (unsafe.expected[1] * 100.0) << '%' << '\n';
		std::cout << net2.CalculateOutputs(safe.input)[0] << ' ' << net2.CalculateOutputs(safe.input)[0] << '\n';
		std::cout << "---------------------------------------------------\n";

		if (index == trainer.GetTrainBatches().size() - 1)
		{
			index = -1;
		}

		if (_kbhit())
			break;
	}
}



























int main(int argc, char** argv)
{
	/*Test();


	return 0;*/



	GEngine::WinInfo info;
	info.clear = GEngine::Color{ 10, 10, 10, 255 };
	info.flags = 0;
	info.width = 1280;
	info.height = 800;
	info.name = "Snake";

	GEngine::Game game{ info };

	MainGame mainGame{ game };
	game.SetFunctions(
		[&mainGame]() { mainGame.Init(); },
		[&mainGame]() { mainGame.Update(); },
		[&mainGame]() { mainGame.Draw(); }
	);

	try
	{
		game.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
	}

	return 0;
}