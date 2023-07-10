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

		// n elements from list
		template<class T>
		static void Sample(const std::vector<T>& list, size_t n, std::vector<T>& out) 
		{
			std::sample(list.begin(), list.end(), std::back_inserter(out), n, rng);
		}

		// samples n elements from list
		template<class T>
		static void Sample(typename std::vector<T>::const_iterator begin, typename std::vector<T>::const_iterator end, size_t n, std::vector<T>& out)
		{
			std::sample(begin, end, std::back_inserter(out), n, rng);
		}
	private:
		static inline std::random_device rd;
		static inline std::mt19937 rng{rd()};
	};
}