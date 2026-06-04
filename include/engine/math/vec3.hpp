#pragma once
#include "engine/utils/type_traits.h"
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
		Vec3 operator=(const Vec3<U>& other);

		Vec3& normalize();

		Vec3 normalized() const;

		Vec3 directionTo(const Vec3& target) const noexcept;

		T distanceTo(const Vec3& other) const noexcept;

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
	T Vec3<T>::distanceTo(const Vec3& other) const noexcept
	{
		return distance(*this, other);
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
	constexpr T LengthSqr(const Vec3<T>& v) noexcept
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	template <Numeric T>
	T length(const Vec2<T>& vec) noexcept
	{
		return std::sqrt(LengthSqr(vec));
	}

	template <Numeric T>
	Vec3<T> normalized(const Vec3<T>& v)
	{
		T len = length(v);
		assert(len != T{ 0 });

		return v / len;
	}

#pragma endregion

#pragma region Operators

	template <Numeric T>
	constexpr Vec3<T> operator-(const Vec3<T>& v) noexcept
	{
		return { -v.x, -v.y, -v.z };
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
	constexpr Vec3<T> operator*(const Vec3<T>& v, U scalar) noexcept
	{
		return { v.x * scalar, v.y * scalar, v.z * scalar };
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T> operator*(U scalar, const Vec3<T>& v) noexcept
	{
		return vec * scalar;
	}

	template <Numeric T>
	constexpr Vec3<T> operator*(const Vec3<T>& lhs, const Vec3<T>& rhs) noexcept
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
	}

	template <Numeric T, Numeric U>
	constexpr Vec3<T> operator/(const Vec3<T>& v, U scalar)
	{
		assert(scalar != T{ 0 });
		return { v.x / scalar, v.y / scalar, v.z / scalar };
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
	constexpr Vec3<T>& operator*=(Vec3<T>& v, U scalar) noexcept
	{
		v.x *= scalar;
		v.y *= scalar;
		v.z *= scalar;

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
	std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}

#pragma endregion
}