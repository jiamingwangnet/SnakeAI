#pragma once

#include "Cost.h"
#include <stdexcept>

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

		T Calculate(Matrix<T> output, Matrix<T> expected) const
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

	template<typename T>
	class CLIP : public Cost<T>
	{
	public:
		CLIP(T advantage, T epsilon) : advantage{ advantage }, epsilon{ epsilon } {}

		T Calculate(T policynew, T policyold) const override
		{
			T ratio = policynew / policyold;

			return std::min(ratio * advantage, std::max( (T)1.0 - epsilon, std::min(ratio, (T)1.0 - epsilon) ) * advantage);
		}

		Matrix<T> Calculate(Matrix<T> policynew, Matrix<T> policyold) const
		{
			Matrix<T> res{ policynew.GetRows(), policyold.GetColumns() };
			for (size_t i = 0; i < policynew.GetSize(); i++)
			{
				res[i] = this->Calculate(policynew[i], policyold[i]);
			}
			return res;
		}

		T Derivative(T policynew, T policyold) const override
		{
			T ratio = policynew / policyold;

			if ((T)1.0 - epsilon <= ratio && ratio <= (T)1.0 + epsilon)
			{
				return (T)1.0 / policyold * advantage;
			}

			return (T)0.0;
		}

		Matrix<T> Derivative(Matrix<T> policynew, Matrix<T> policyold) const override
		{
			Matrix<T> res{ policynew.GetRows(), policyold.GetColumns() };
			for (size_t i = 0; i < policynew.GetSize(); i++)
			{
				res[i] = this->Derivative(policynew[i], policyold[i]);
			}
			return res;
		}
	private:
		T advantage;
		T epsilon;
	};

	template<class T>
	class CLIPVFS : public Cost<T>
	{
	public:
		CLIPVFS(T advantage, T epsilon, T c1, T c2, T entropy, T entropyDeriv) : clp{ advantage, epsilon }, entropy{ entropy }, entropyDeriv{ entropyDeriv }, c1{c1}, c2{c2} {}

		T Calculate(T policynew, T policyold, T valuefunc, T valuetarg) const
		{
			return clp.Calculate(policynew, policyold) - c1 * mse.Calculate(valuefunc, valuetarg) + c2 * entropy;
		}

		Matrix<T> Calculate(Matrix<T> policynew, Matrix<T> policyold, Matrix<T> valuefunc, Matrix<T> valuetarg) const
		{
			Matrix<T> res{ policynew.GetRows(), policyold.GetColumns() };
			for (size_t i = 0; i < policynew.GetSize(); i++)
			{
				res[i] = this->Calculate(policynew[i], policyold[i], valuefunc[i], valuetarg[i]);
			}
			return res;
		}

		T Derivative(T policynew, T policyold, T valuefunc, T valuetarg) const
		{
			return clp.Derivative(policynew, policyold) - c1 * mse.Derivative(valuefunc, valuetarg) + c2 * entropyDeriv; // TODO: figure out how to differentiate entropy
		}

		Matrix<T> Derivative(Matrix<T> policynew, Matrix<T> policyold, Matrix<T> valuefunc, Matrix<T> valuetarg) const
		{
			Matrix<T> res{ policynew.GetRows(), policyold.GetColumns() };
			for (size_t i = 0; i < policynew.GetSize(); i++)
			{
				res[i] = this->Derivative(policynew[i], policyold[i], valuefunc[i], valuetarg[i]);
			}
			return res;
		}

		T Calculate(T policynew, T policyold) const override
		{
			throw std::logic_error{ "NOT IMPLEMENTED." };
		}

		T Derivative(T policynew, T policyold) const override
		{
			throw std::logic_error{ "NOT IMPLEMENTED." };
		}

		Matrix<T> Derivative(Matrix<T> policynew, Matrix<T> policyold) const override
		{
			throw std::logic_error{ "NOT IMPLEMENTED." };
		}

	private:
		T entropy;
		T entropyDeriv;

		MSE<T> mse;
		CLIP<T> clp;

		T c1;
		T c2;
	};
}