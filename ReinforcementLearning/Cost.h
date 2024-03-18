#pragma once

#include "Matrix.h"

namespace net
{
	template<typename T>
	class Cost
	{
	public:
		virtual T Calculate(T predicted, T expected) const = 0;
		virtual T Derivative(T predicted, T expected) const = 0;
		virtual Matrix<T> Derivative(Matrix<T> output, Matrix<T> expected) const = 0; // gets the derivative for each row/column
	};
}