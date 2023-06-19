#pragma once

#include <vector>
#include <memory>
#include "Layer.h"
#include "Cost.h"
#include <string>

namespace dqn
{
	class NeuralNet
	{
	public:
		struct DataPoint
		{
			DMatrix input;
			DMatrix predicted;
			DMatrix expected;
		};
	public:
		NeuralNet(const std::vector<LayerInfo>& layerinfo);
		NeuralNet(const NeuralNet& rhs)
		{
			layers.resize(rhs.layers.size());

			LayerInfo info;
			info.activation = rhs.layers[0]->GetActivation();
			info.in_nodes = 0;
			info.nodes = rhs.layers[0]->GetNodes();

			layers[0] = std::make_unique<Layer>(info);
			layers[0]->SetWeights(rhs.layers[0]->GetWeights());
			layers[0]->SetBiases(rhs.layers[0]->GetBiases());

			for (int i = 1; i < rhs.layers.size(); i++)
			{
				LayerInfo info;
				info.activation = rhs.layers[i]->GetActivation();
				info.in_nodes = rhs.layers[i - 1]->GetNodes();
				info.nodes = rhs.layers[i]->GetNodes();

				layers[i] = std::make_unique<Layer>(info);
				layers[i]->SetWeights(rhs.layers[i]->GetWeights());
				layers[i]->SetBiases(rhs.layers[i]->GetBiases());
			}
		}
		NeuralNet& operator=(const NeuralNet& rhs)
		{
			layers.resize(rhs.layers.size());

			LayerInfo info;
			info.activation = rhs.layers[0]->GetActivation();
			info.in_nodes = 0;
			info.nodes = rhs.layers[0]->GetNodes();

			layers[0] = std::make_unique<Layer>(info);
			layers[0]->SetWeights(rhs.layers[0]->GetWeights());
			layers[0]->SetBiases(rhs.layers[0]->GetBiases());

			for (int i = 1; i < rhs.layers.size(); i++)
			{
				LayerInfo info;
				info.activation = rhs.layers[i]->GetActivation();
				info.in_nodes = rhs.layers[i - 1]->GetNodes();
				info.nodes = rhs.layers[i]->GetNodes();

				layers[i] = std::make_unique<Layer>(info);
				layers[i]->SetWeights(rhs.layers[i]->GetWeights());
				layers[i]->SetBiases(rhs.layers[i]->GetBiases());
			}

			return *this;
		}

		void Save(std::string path) const;
		void Load(std::string path);

		DMatrix CalculateOutputs(const DMatrix& input);
		void GradientDescent(double learningRate, DataPoint& dataPoint, const Cost<double>& cost);
		void GradientDescent(double learningRate, std::vector<DataPoint>& batch, const Cost<double>& cost);

	private:
		void GetGradients(DataPoint& dataPoint, const Cost<double>& cost);
		DMatrix OutputLayerValues(const DMatrix& costDerivative);
		DMatrix HiddenLayerValues(size_t layer_i, const DMatrix& nodeValues);
		void UpdateGradients(size_t layer, const DMatrix& nodeValues);
		void ClearGradients();

	private:
		std::vector<std::unique_ptr<Layer>> layers;
		std::vector<LayerInfo> layerInfo;

		std::vector<DMatrix> weightGrad;
		std::vector<DMatrix> biasGrad;

		// temporary fix for storing loaded activation functions
		std::vector<std::shared_ptr<Activation>> activs;

		static constexpr double BIAS = 0.0;
	};
}