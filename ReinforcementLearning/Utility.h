#pragma once

#include <random>
#include <iostream>
#include <vector>

namespace net
{
	class Util
	{
	public:
		template<class T, class Dist>
		static T Random(Dist dist)
		{
			return dist(rng);
		}

		static bool RandomChance(double chance)
		{
			return Random<double>(std::uniform_real_distribution<double>(0.0, 1.0)) <= chance;
		}

		static size_t RandomFromDistribution(std::vector<double>& distribution)
		{
			double rand = Random<double>(std::uniform_real_distribution<double>(0.0, 1.0));
			double accum = 0.0;

			for (int i = 0; i < distribution.size(); i++)
			{
				double& prob = distribution[i];

				accum += prob;
				if (rand < accum)
					return i;
			}

			// account for floating point errors of accum
			return distribution.size() - 1; // may introduce bias
		}

		// n elements from list
		template<class T>
		static void Sample(const std::vector<T>& list, size_t n, std::vector<T>& out) 
		{
			std::sample(list.begin(), list.end(), std::back_inserter(out), n, rng);
		}

		// samples n elements from list
		template<class T, class Cont>
		static void Sample(typename Cont::const_iterator begin, typename Cont::const_iterator end, size_t n, std::vector<T>& out)
		{
			std::sample(begin, end, std::back_inserter(out), n, rng);
		}

		template<class T>
		static T LogBase(T b, T x)
		{
			return log2(x) / log2(b);
		}
	private:
		static inline std::random_device rd;
		static inline std::mt19937 rng{rd()};
	};
}