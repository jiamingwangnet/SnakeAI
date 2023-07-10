#pragma once

#include "Cost.h"

namespace net
{
	template<typename T>
	class MSE : public Cost<T>
	{
	public:
		T Calculate(T predicted, T expected) const override
		{
			return (predicted - expected) * (predicted - expected);
		}

		T Calculate(Matrix<T> output, Matrix<T> expected) const override
		{
			T sum = 0.0;
			for (size_t i = 0; i < expected.GetSize(); i++)
			{
				sum += this->Calculate(output[i], expected[i]);
			}
			return sum;
		}

		T Derivative(T predicted, T expected) const override
		{
			return 2.0 * (predicted - expected);
		}

		Matrix<T> Derivative(Matrix<T> output, Matrix<T> expected) const override
		{
			Matrix<T> res{ expected.GetRows(), expected.GetColumns() };
			for (size_t i = 0; i < expected.GetSize(); i++)
			{
				res[i] = this->Derivative(output[i], expected[i]);
			}
			return res;
		}
	};
}