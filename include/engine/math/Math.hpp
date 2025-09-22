#pragma once
#include "engine/utils/type_traits.h"
#include <cassert>

namespace cursed_engine
{
	template <Numeric T>
	constexpr T min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <Numeric T>
	constexpr T max(T a, T b)
	{
		return b < a ? a : b;
	}

	template <SignedNumeric T>
	constexpr T abs(T value)
	{
		return value < 0 ? -value : value;
	}

	template <Numeric T>
	constexpr T clamp(T value, T min, T max)
	{
		assert(max >= min);
		return std::max(min, std::min(max, value));
	}

	// only have (?) - remove min max abs?
	// clamp

	// lerp

	// inv_lerp

	// wrap

	// remap

		
}