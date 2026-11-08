#pragma once
#include "engine/utils/concepts.h"
#include <cassert>
#include <ostream>

namespace cursed_engine
{
	template <Numeric T>
	struct Vec3
	{
		constexpr Vec3() = default;
		constexpr Vec3(T x, T y, T z);
		constexpr Vec3(const Vec3&) = default;
		constexpr Vec3(Vec3&&) = default;

		template <Numeric U>
		constexpr explicit Vec3(const Vec3<U>& other);

		Vec3& operator=(const Vec3&) = default;
		Vec3& operator=(Vec3&&) = default;

		template <Numeric U>
		Vec3& operator=(const Vec3<U>& other);

		Vec3& normalize();

		Vec3 normalized() const;

		Vec3 directionTo(const Vec3& target) const noexcept;

		T distanceTo(const Vec3& taget) const noexcept;

		T x{}, y{}, z{};
	};

	template <Numeric T>
	Vec3(T, T) -> Vec3<T>;

	using IVec3 = Vec3<int>;
	using FVec3 = Vec3<float>;

#pragma region Definitions

	template <Numeric T>
	constexpr Vec3<T>::Vec3(T x, T y, T z)
		: x{ x }, y{ y }, z{ z }
		{
		}

	template <Numeric T>
	template <Numeric U>
	constexpr Vec3<T>::Vec3(const Vec3<U>& other)
	: x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) }, z{ static_cast<T>(other.z) }
	{
	}

	template <Numeric T>
	template <Numeric U>
	Vec3<T>& Vec3<T>::operator=(const Vec3<U>& other)
	{
		x = static_cast<T>(other.x);
		y = static_cast<T>(other.y);
		z = static_cast<T>(other.z);

		return *this;
	}

	template <Numeric T>
	Vec3<T>& Vec3<T>::normalize()
	{
		T len = length(*this);
		assert(len != T{ 0 });

		x /= len;
		y /= len;
		z /= len;

		return *this;
	}

	template <Numeric T>
	Vec3<T> Vec3<T>::normalized() const
	{
		return Vec3(*this).normalize();
	}

	template <Numeric T>
	Vec3<T> Vec3<T>::directionTo(const Vec3& target) const noexcept
	{
		return direction(*this, target);
	}

	template <Numeric T>
	T Vec3<T>::distanceTo(const Vec3& target) const noexcept
	{
		return distance(*this, target);
	}

#pragma endregion

#pragma region Helpers

	template <Numeric T>
	Vec3<T> direction(const Vec3<T>& from, const Vec3<T>& to)
	{
		Vec3<T> result = to - from;
		T len = length(result);

		if (len != T{ 0 })
		{
			result.x /= len;
			result.y /= len;
			result.z /= len;
		}

		return result;
	}

	template <Numeric T>
	T distance(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return length(lhs - rhs);
	}

	template <Numeric T>
	constexpr T dot(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
	}

	template <Numeric T>
	constexpr T LengthSqr(const Vec3<T>& vec) noexcept
	{
		return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	}

	template <Numeric T>
	T length(const Vec3<T>& vec) noexcept
	{
		return std::sqrt(LengthSqr(vec));
	}

	template <Numeric T>
	Vec3<T> normalized(const Vec3<T>& vec)
	{
		T len = length(vec);
		assert(len != T{ 0 });

		return vec / len;
	}

#pragma endregion

#pragma region Operators

	template <Numeric T>
	constexpr Vec3<T> operator-(const Vec3<T>& vec) noexcept
	{
		return { -vec.x, -vec.y, -vec.z };
	}

	template <Numeric T>
	constexpr Vec3<T> operator+(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	template <Numeric T>
	constexpr Vec3<T> operator-(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T> operator*(const Vec3<T>& vec, U scalar) noexcept
	{
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T> operator*(U scalar, const Vec3<T>& vec) noexcept
	{
		return vec * scalar;
	}

	template <Numeric T>
	constexpr Vec3<T> operator*(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T> operator/(const Vec3<T>& vec, U scalar)
	{
		assert(scalar != T{ 0 });
		return { vec.x / scalar, vec.y / scalar, vec.z / scalar };
	}

	template <Numeric T>
	constexpr Vec3<T>& operator+=(Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;

		return lhs;
	}

	template <Numeric T>
	constexpr Vec3<T>& operator-=(Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;

		return lhs;
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T>& operator*=(Vec3<T>& vec, U scalar) noexcept
	{
		vec.x *= scalar;
		vec.y *= scalar;
		vec.z *= scalar;

		return vec;
	}

	template <Numeric T>
	constexpr Vec3<T>& operator*=(Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;

		return lhs;
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T>& operator/=(Vec3<T>& vec, U scalar)
	{
		assert(scalar != T{ 0 });

		vec.x /= scalar;
		vec.y /= scalar;
		vec.z /= scalar;

		return vec;
	}

	template <Numeric T>
	constexpr bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
	}

	template <Numeric T>
	constexpr bool operator!=(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <Numeric T>
	std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec)
	{
		os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
		return os;
	}

#pragma endregion
}