#pragma once

#include "Matrix.h"
#include <memory>

namespace dqn
{
	enum class ACTIVATION_TYPE
	{
		SIGMOID,
		RELU,
		SOFTMAX,
		LINEAR
	};

	class Activation
	{
	public:
		virtual Matrix<double> Activate(const Matrix<double>& nodes) const = 0;
		virtual Matrix<double> Derivative(const Matrix<double>& nodes) const = 0;
		virtual ACTIVATION_TYPE GetType() const = 0;
	};
}