#pragma once
#include "engine/utils/concepts.h"

namespace cursed_engine
{	
	template <Numeric T>
	struct Rect
	{
		constexpr Rect() = default; // needed?

		constexpr Rect(T x, T y, T w, T h);

		template <Numeric U>
		constexpr Rect(const Rect<U>& other);

		T x{};
		T y{};
		T w{};
		T h{};
	};

	template <Numeric T>
	Rect(T, T, T, T) -> Rect<T>;

	using FRect = Rect<float>;
	using IRect = Rect<int>;

#pragma region Definitions

	template <Numeric T>
	constexpr Rect<T>::Rect(T x, T y, T w, T h)
		: x{ x }, y{ y }, w{ w }, h{ h }
	{
	}

	template <Numeric T>
	template <Numeric U>
	constexpr Rect<T>::Rect(const Rect<U>& other)
		: x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) }, w{ static_cast<T>(other.w) }, h{ static_cast<T>(other.h) }
	{
	}

#pragma endregion
}