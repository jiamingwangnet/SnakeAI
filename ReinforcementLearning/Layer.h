#pragma once

#include "Activation.h"
#include <memory>


/*
	When using the wrong weighted outputs and inputs, performing gradient descent will result in -nan(ind)
*/
namespace net
{
	struct LayerInfo
	{
		size_t nodes;
		size_t in_nodes;
		std::shared_ptr<Activation> activation;
	};

	class Layer
	{
	public:
		Layer(LayerInfo info);
		Layer() = default;

		DMatrix Forward(const DMatrix& input, bool start = false);
	public:
		const DMatrix& GetWeights() const;
		void SetWeights(const DMatrix& value);

		const DMatrix& GetBiases() const;
		void SetBiases(const DMatrix& value);

		const DMatrix& GetWeightedInputs() const;
		const DMatrix& GetOutputs() const;
		
		// for gradient descent
		void SetWeightedInputs(const DMatrix& val)
		{
			weightedInputs = val;
		}

		void SetOutputs(const DMatrix& val)
		{
			outputs = val;
		}

		std::shared_ptr<Activation> GetActivation() const { return activation; }
		void SetActivation(std::shared_ptr<Activation>& activation) { this->activation = activation; }
		size_t GetNodes() const { return n_nodes; }
	private:
		static constexpr double WEIGHT_MIN = -1.0;
		static constexpr double WEIGHT_MAX = 1.0;

		size_t n_nodes = 0;
		DMatrix weights; // inputs x outputs
		DMatrix biases;
		DMatrix weightedInputs{};
		DMatrix outputs{};
		std::shared_ptr<Activation> activation;
	};
}

