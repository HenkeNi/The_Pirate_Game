#pragma once
#include "engine/utils/type_traits.h"
#include <cassert>
#include <ostream>
#include <cmath>

namespace cursed_engine
{
	template <Numeric T>
	struct Vec2
	{
		constexpr Vec2() = default;
		constexpr Vec2(T x_, T y_) : x{ x_ }, y{ y_ } {}

		T distanceTo(const Vec2& other) const noexcept;

		Vec2 directionTo(const Vec2& target) const;
		
		Vec2& normalize();
		
		Vec2 normalized() const;

		T x{};
		T y{};
	};

	template <Numeric T>
	Vec2(T, T) -> Vec2<T>;

	using IVec2 = Vec2<int>; 
	using FVec2 = Vec2<float>;

	// ==================== Unary Operators ====================

	template <Numeric T>
	constexpr Vec2<T> operator-(const Vec2<T>& vec) noexcept
	{
		return { -vec.x, -vec.y };
	}

	// ==================== Arithmetic Operators ====================

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
		
	template <Numeric T>
	constexpr Vec2<T> operator*(const Vec2<T>& vec, T scalar) noexcept
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	template <Numeric T>
	constexpr Vec2<T> operator*(T scalar, const Vec2<T>& vec) noexcept
	{
		return vec * scalar;
	}
		
	template <Numeric T>
	constexpr Vec2<T> operator*(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y };
	}
		
	template <Numeric T>
	constexpr Vec2<T> operator/(const Vec2<T>& vec, T scalar)
	{
		assert(scalar != T{ 0 });
		return { vec.x / scalar, vec.y / scalar };
	}
		
	// ==================== Compound Assignment ====================

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
	
	template <Numeric T>
	constexpr Vec2<T>& operator*=(Vec2<T>& vec, T scalar) noexcept
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

	template <Numeric T>
	constexpr Vec2<T>& operator/=(Vec2<T>& vec, T scalar)
	{
		assert(scalar != T{ 0 });

		vec.x /= scalar;
		vec.y /= scalar;

		return vec;
	}
	
	// ==================== Comparison ====================

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

	// ==================== Output ====================

	template <Numeric T>
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec)
	{
		os << "[" << vec.x << ", " << vec.y << "]";
		return os;
	}

	// ==================== Utility Functions ====================
	
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
	T distance(const Vec2<T>& lhs, const Vec2<T>& rhs) noexcept
	{
		return length(lhs - rhs);
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

	// ==================== Member Functions ====================

	template <Numeric T>
	T Vec2<T>::distanceTo(const Vec2<T>& other) const noexcept
	{
		return distance(*this, other);
	}

	template <Numeric T>
	Vec2<T> Vec2<T>::directionTo(const Vec2<T>& target) const
	{
		return direction(*this, target);
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
}