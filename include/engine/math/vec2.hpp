#pragma once
#include "engine/utils/concepts.h"
#include <cassert>
#include <ostream>
#include <cmath>

namespace cursed_engine
{
	// Maybe more overloads should use 'template <Numeric T, Numeric U>'?

	template <Numeric T>
	struct Vec2
	{
		constexpr Vec2() = default;
		constexpr Vec2(T x, T y);
		constexpr Vec2(const Vec2&) = default;
		constexpr Vec2(Vec2&&) = default;

		template <Numeric U>
		constexpr explicit Vec2(const Vec2<U>& other);

		Vec2& operator=(const Vec2&) = default;
		Vec2& operator=(Vec2&&) = default;

		template <Numeric U>
		Vec2 operator=(const Vec2<U>& other);
		
		Vec2& normalize();
		
		Vec2 normalized() const;

		Vec2 directionTo(const Vec2& target) const;

		T distanceTo(const Vec2& other) const noexcept;

		T x{};
		T y{};
	};

	template <Numeric T>
	Vec2(T, T) -> Vec2<T>;

	using IVec2 = Vec2<int>; 
	using FVec2 = Vec2<float>;

#pragma region Definitions

	template <Numeric T>
	constexpr Vec2<T>::Vec2(T x, T y) 
		: x{ x }, y{ y }
	{
	}

	template <Numeric T>
	template <Numeric U>
	constexpr Vec2<T>::Vec2(const Vec2<U>& other)
		: x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) }
	{
	}

	template <Numeric T>
	template <Numeric U>
	Vec2<T> Vec2<T>::operator=(const Vec2<U>& other)
	{
		x = static_cast<T>(other.x);
		y = static_cast<T>(other.y);

		return *this;
	}

	template <Numeric T>
	Vec2<T>& Vec2<T>::normalize()
	{
		T len = length(*this);
		assert(len != T{ 0 });

		x /= len;
		y /= len;

		return *this;
	}

	template <Numeric T>
	Vec2<T> Vec2<T>::normalized() const
	{
		return Vec2(*this).normalize();
	}

	template <Numeric T>
	Vec2<T> Vec2<T>::directionTo(const Vec2<T>& target) const
	{
		return direction(*this, target);
	}

	template <Numeric T>
	T Vec2<T>::distanceTo(const Vec2<T>& other) const noexcept
	{
		return distance(*this, other);
	}

#pragma endregion

#pragma region Helpers

	template <Numeric T>
	constexpr T dot(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	}

	template <Numeric T>
	constexpr T LengthSqr(const Vec2<T>& vec) noexcept
	{
		return (vec.x * vec.x) + (vec.y * vec.y);
	}

	template <Numeric T>
	T length(const Vec2<T>& vec) noexcept
	{
		return std::sqrt(LengthSqr(vec));
	}

	template <Numeric T>
	Vec2<T> normalized(const Vec2<T>& vec)
	{
		T len = length(vec);
		assert(len != T{ 0 });

		return vec / len;
	}

	template <Numeric T>
	Vec2<T> direction(const Vec2<T>& from, const Vec2<T>& to)
	{
		Vec2<T> result = to - from;
		T len = length(result);

		if (len != T{ 0 })
		{
			result.x /= len;
			result.y /= len;
		}

		return result;
	}

	template <Numeric T>
	T distance(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return length(lhs - rhs);
	}

#pragma endregion

#pragma region Operators

	template <Numeric T>
	constexpr Vec2<T> operator-(const Vec2<T>& vec) noexcept
	{
		return { -vec.x, -vec.y };
	}

	template <Numeric T>
	constexpr Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}

	template <Numeric T>
	constexpr Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}
		
	template <Numeric T, Numeric U>
	constexpr Vec2<T> operator*(const Vec2<T>& vec, U scalar) noexcept
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	template <Numeric T, Numeric U>
	constexpr Vec2<T> operator*(U scalar, const Vec2<T>& vec) noexcept
	{
		return vec * scalar;
	}
		
	template <Numeric T>
	constexpr Vec2<T> operator*(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y };
	}
		
	template <Numeric T, Numeric U>
	constexpr Vec2<T> operator/(const Vec2<T>& vec, U scalar)
	{
		assert(scalar != T{ 0 });
		return { vec.x / scalar, vec.y / scalar };
	}
		
	template <Numeric T>
	constexpr Vec2<T>& operator+=(Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;

		return lhs;
	}
	
	template <Numeric T>
	constexpr Vec2<T>& operator-=(Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;

		return lhs;
	}
	
	template <Numeric T, Numeric U>
	constexpr Vec2<T>& operator*=(Vec2<T>& vec, U scalar) noexcept
	{
		vec.x *= scalar;
		vec.y *= scalar;
		
		return vec;
	}

	template <Numeric T>
	constexpr Vec2<T>& operator*=(Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		
		return lhs;
	}

	template <Numeric T, Numeric U>
	constexpr Vec2<T>& operator/=(Vec2<T>& vec, U scalar)
	{
		assert(scalar != T{ 0 });

		vec.x /= scalar;
		vec.y /= scalar;

		return vec;
	}
	
	template <Numeric T>
	constexpr bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}

	template <Numeric T>
	constexpr bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <Numeric T>
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec)
	{
		os << "[" << vec.x << ", " << vec.y << "]";
		return os;
	}

#pragma endregion
}