#pragma once

#include <vector>
#include <memory>
#include "Layer.h"
#include "Cost.h"
#include <string>


/*
	When using the wrong weighted outputs and inputs, performing gradient descent will result in -nan(ind)
*/
namespace net
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
		NeuralNet() = default;
		NeuralNet(const NeuralNet& rhs)
		{
			layers.resize(rhs.layers.size());

			weightGrad.resize(rhs.layerInfo.size());
			biasGrad.resize(rhs.layerInfo.size());

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

			this->layerInfo = rhs.layerInfo;

			ClearGradients();
		}
		NeuralNet& operator=(const NeuralNet& rhs)
		{
			layers.resize(rhs.layers.size());

			weightGrad.resize(rhs.layerInfo.size());
			biasGrad.resize(rhs.layerInfo.size());

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

			this->layerInfo = rhs.layerInfo;

			ClearGradients();
				
			return *this;
		}

		void Copy(const NeuralNet& rhs)
		{
			layers[0]->SetWeights(rhs.layers[0]->GetWeights());
			layers[0]->SetBiases(rhs.layers[0]->GetBiases());

			for (int i = 1; i < rhs.layers.size(); i++)
			{
				layers[i]->SetWeights(rhs.layers[i]->GetWeights());
				layers[i]->SetBiases(rhs.layers[i]->GetBiases());
			}
		}

		void ChangeLayerActivation(size_t layer, std::shared_ptr<Activation> activ)
		{
			layers[layer]->SetActivation(activ);
		}

		void Save(std::string path) const;
		void Load(std::string path);

		DMatrix CalculateOutputs(const DMatrix& input);
		void GradientDescent(double learningRate, const DMatrix& costGrad);
		void GradientDescent(double learningRate, const std::vector<DMatrix>& costGrads);

		std::vector<std::unique_ptr<Layer>>& GetLayers() { return layers; }
	private:
		void GetGradients(const DMatrix& costGrad);
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
