#pragma once
#include <random>

// Random should use a seed!

namespace cursed_engine::random
{
	template <typename T>
	T generateRandomInteger(T min, T max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());

		std::uniform_int_distribution<T> dist(min, max);
		return dist(gen);
	}

	template <typename T>
	T generateRandomFloatingPoint(T min, T max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());

		std::uniform_real_distribution<T> dis(min, max);
		return dis(gen);
	}
}