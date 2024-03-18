#include "NeuralNet.h"
#include <fstream>
#include "ActivationFuncs.h"
#include <iostream>

#ifdef DEBUG_LOG

#define FILEOUT "debug.txt"
#endif

net::NeuralNet::NeuralNet(const std::vector<LayerInfo>& layerinfo)
	: layerInfo{layerinfo}
{
	weightGrad.resize(layerInfo.size());
	biasGrad.resize(layerInfo.size());

	layers.resize(layerinfo.size());
	for (int i = 0; i < layerinfo.size(); i++)
	{
		layers[i] = std::make_unique<Layer>(layerinfo[i]);
	}

	ClearGradients();
}

net::DMatrix net::NeuralNet::CalculateOutputs(const DMatrix& input)
{
	DMatrix output;

	if (input.GetColumns() != layers[0]->GetNodes())
	{
		throw "AAAAAAA";
		return DMatrix{ 1, 1, 0.0 };
	}

	output = layers[0]->Forward(input, true);

	for (auto it = layers.begin() + 1; it != layers.end(); ++it)
		output = (*it)->Forward(output);

	return output;
}

void net::NeuralNet::Save(std::string path) const
{
	std::ofstream out{ path, std::ios::binary };
	if (!out) std::cout << path + " could not be opened.";

	bool f = true;
	for (const std::unique_ptr<Layer>& layer : layers)
	{
		if (f)
		{
			f = false; continue;
		}
		for (const double& w : layer->GetWeights())
		{
			out.write(reinterpret_cast<const char*>(&w), sizeof(w));
		}
		for (const double& b : layer->GetBiases())
		{
			out.write(reinterpret_cast<const char*>(&b), sizeof(b));
		}
	}
	out.close();
}

void net::NeuralNet::Load(std::string path) // FIXME
{
	std::ifstream in{ path, std::ios::binary };
	if (!in) std::cout << path + " could not be opened.";

	ClearGradients();

	for (int i = 1; i < layers.size(); i++)
	{
		DMatrix weight{ layers[i]->GetWeights().GetRows(), layers[i]->GetWeights().GetColumns() };
		DMatrix bias{ layers[i]->GetBiases().GetRows(), layers[i]->GetBiases().GetColumns() };

		for (int j = 0; j < layers[i]->GetWeights().GetSize(); j++)
		{
			in.read(reinterpret_cast<char*>(&(weight[j])), sizeof(double));
		}
		for (int j = 0; j < layers[i]->GetBiases().GetSize(); j++)
		{
			in.read(reinterpret_cast<char*>(&(bias[j])), sizeof(double));
		}

		layers[i]->SetWeights(weight);
		layers[i]->SetBiases(bias);
	}

	in.close();
}

void net::NeuralNet::GradientDescent(double learningRate, const DMatrix& costGrad)
{
	GetGradients(costGrad);

	for (size_t i = 1; i < layers.size(); i++)
	{
		std::unique_ptr<Layer>& layer = layers[i];
		layer->SetWeights(layer->GetWeights() - weightGrad[i] * learningRate);
		layer->SetBiases(layer->GetBiases() - biasGrad[i] * learningRate);
	}
	
	#ifdef DEBUG_LOG
	std::ofstream debugFile{ FILEOUT, std::ios_base::app };

	for (int i = 1; i < layers.size(); i++)
	{
		std::unique_ptr<Layer>& layer = layers[i];
		debugFile << "Layer " << i << ": \n";

		debugFile << "\tWeights Grad: \n\t\t";
		for (const double& val : weightGrad[i])
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';

		debugFile << "\tBiases Grad: \n\t\t";
		for (const double& val : biasGrad[i])
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';

		debugFile << "\tWeights: \n\t\t";
		for (const double& val : layer->GetWeights())
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';

		debugFile << "\tBiases: \n\t\t";
		for (const double& val : layer->GetBiases())
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';
	}
	#endif

	ClearGradients();
}

void net::NeuralNet::GradientDescent(double learningRate, const std::vector<DMatrix>& costGrads)
{
	for (const DMatrix& costGrad : costGrads)
	{
		GetGradients(costGrad);
	}

	for (size_t i = 1; i < layers.size(); i++)
	{
		std::unique_ptr<Layer>& layer = layers[i];
		layer->SetWeights(layer->GetWeights() - weightGrad[i] * learningRate);
		layer->SetBiases(layer->GetBiases() - biasGrad[i] * learningRate);
	}

#ifdef DEBUG_LOG
	std::ofstream debugFile{ FILEOUT, std::ios_base::app };

	for (int i = 1; i < layers.size(); i++)
	{
		std::unique_ptr<Layer>& layer = layers[i];
		debugFile << "Layer " << i << ": \n";

		debugFile << "\tWeights Grad: \n\t\t";
		for (const double& val : weightGrad[i])
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';

		debugFile << "\tBiases Grad: \n\t\t";
		for (const double& val : biasGrad[i])
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';

		debugFile << "\tWeights: \n\t\t";
		for (const double& val : layer->GetWeights())
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';

		debugFile << "\tBiases: \n\t\t";
		for (const double& val : layer->GetBiases())
		{
			debugFile << val << ' ';
		}
		debugFile << '\n';
	}
#endif

	ClearGradients();
}

void net::NeuralNet::GetGradients(const DMatrix& costGrad)
{
	DMatrix nodeValues = OutputLayerValues(costGrad);
	UpdateGradients(layers.size() - 1, nodeValues);

	for (size_t i = layers.size() - 2; i > 0; --i)
	{
		nodeValues = HiddenLayerValues(i, nodeValues);
		UpdateGradients(i, nodeValues);
	}
}

net::DMatrix net::NeuralNet::OutputLayerValues(const DMatrix& costDerivative)
{
	return layers.back()->GetActivation()->
		Derivative(layers.back()->GetWeightedInputs())
		.Hadamard(costDerivative);
}

net::DMatrix net::NeuralNet::HiddenLayerValues(size_t layer_i, const DMatrix& nodeValues)
{
	return (nodeValues * layers[layer_i + 1]->GetWeights().GetTransposed())
			.Hadamard(
			layers[layer_i]->GetActivation()
			->Derivative(
				layers[layer_i]->GetWeightedInputs()
			));
}

void net::NeuralNet::UpdateGradients(size_t layer, const DMatrix& nodeValues)
{
	weightGrad[layer] += layers[layer - 1]->GetOutputs().GetTransposed() * nodeValues;
	biasGrad[layer] += nodeValues;
}

void net::NeuralNet::ClearGradients()
{
	for (auto grad = weightGrad.begin(); grad != weightGrad.end(); ++grad)
	{
		size_t i = grad - weightGrad.begin();
		*grad = DMatrix{ layers[i]->GetWeights().GetRows(), layers[i]->GetWeights().GetColumns() };
	}

	for (auto grad = biasGrad.begin(); grad != biasGrad.end(); ++grad)
	{
		size_t i = grad - biasGrad.begin();
		*grad = DMatrix{ layers[i]->GetBiases().GetRows(), layers[i]->GetBiases().GetColumns() };
	}
}
