#pragma once
#include "engine/utils/concepts.h"
#include "engine/math/vec2.hpp"

namespace cursed_engine
{
	template <Numeric T>
	struct AABB
	{
		AABB() = default;

		AABB(T x, T y, T w, T h)
			: min{ x, y }, max{ x + w, y + h }
		{
		}

		AABB(Vec2<T> min, Vec2<T> max)
			: min{ min }, max{ max }
		{
		}

		[[nodiscard]] bool contains(const Vec2<T>& point) const noexcept;
		[[nodiscard]] bool intersects(const AABB<T>& other) const noexcept;

		[[nodiscard]] Vec2<T> center() const noexcept;

		Vec2<T> min;
		Vec2<T> max;
	};

	template <Numeric T>
	AABB(T, T) -> AABB<T>;

	using IAABB = AABB<int>;
	using FAABB = AABB<float>;

	using Bounds = AABB<float>;

#pragma region Definitions

	template <Numeric T>
	bool AABB<T>::contains(const Vec2<T>& point) const noexcept
	{
		return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y;
	}

	template <Numeric T>
	bool AABB<T>::intersects(const AABB<T>& other) const noexcept
	{
		return min.x <= other.max.x &&
			max.x >= other.min.x &&
			min.y <= other.max.y &&
			max.y >= other.min.y;
	}

	template <Numeric T>
	Vec2<T> AABB<T>::center() const noexcept
	{
		return min + (max - min) * static_cast<T>(0.5);
	}

#pragma endregion
}