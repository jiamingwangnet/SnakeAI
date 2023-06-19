#pragma once

#include "Activation.h"
#include <cmath>
#include <memory>

namespace dqn
{
	class Sigmoid : public Activation
	{
	public:
		Matrix<double> Activate(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };
			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				res[i] = 1.0 / (1.0 + std::exp(-nodes[i]));
			}
			return res;
		}

		Matrix<double> Derivative(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };
			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				double activation = 1.0 / (1.0 + std::exp(-nodes[i]));
				res[i] = activation * (1.0 - activation);
			}
			return res;
		}

		ACTIVATION_TYPE GetType() const override
		{
			return ACTIVATION_TYPE::SIGMOID;
		}
	};

	class ReLU : public Activation
	{
		Matrix<double> Activate(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };
			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				res[i] = std::max(0.0, nodes[i]);
			}
			return res;
		}

		Matrix<double> Derivative(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };
			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				res[i] = nodes[i] <= 0.0 ? 0.0 : 1.0;
			}
			return res;
		}

		ACTIVATION_TYPE GetType() const override
		{
			return ACTIVATION_TYPE::RELU;
		}
	};

	class Linear : public Activation
	{
		Matrix<double> Activate(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };
			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				res[i] = nodes[i];
			}
			return res;
		}

		Matrix<double> Derivative(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };

			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				res[i] = 1.0;
			}
			return res;
		}

		ACTIVATION_TYPE GetType() const override
		{
			return ACTIVATION_TYPE::LINEAR;
		}
	};

	class Softmax : public Activation
	{
		Matrix<double> Activate(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };
			double expSum = 0.0;
			for (const double& v : nodes)
			{
				expSum += std::exp(v);
			}
			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				res[i] = std::exp(nodes[i]) / expSum;
			}
			return res;
		}

		Matrix<double> Derivative(const Matrix<double>& nodes) const override
		{
			Matrix<double> res{ nodes.GetRows(), nodes.GetColumns() };

			double sum = 0.0;
			for (const double& v : nodes)
			{
				sum += std::exp(v);
			}

			for (size_t i = 0; i < nodes.GetSize(); i++)
			{
				double ex = std::exp(nodes[i]);

				res[i] = (ex * sum - ex * ex) / (sum * sum);
			}
			return res;
		}

		ACTIVATION_TYPE GetType() const override
		{
			return ACTIVATION_TYPE::SOFTMAX;
		}
	};

	inline std::shared_ptr<Activation> GetActivation(ACTIVATION_TYPE type)
	{
		switch (type)
		{
		case ACTIVATION_TYPE::SIGMOID:
			return std::make_shared<Sigmoid>();
		case ACTIVATION_TYPE::RELU:
			return std::make_shared<ReLU>();
		case ACTIVATION_TYPE::SOFTMAX:
			return std::make_shared<Softmax>();
		case ACTIVATION_TYPE::LINEAR:
			return std::make_shared<Linear>();
		default:
			return nullptr;
		}
	}
}