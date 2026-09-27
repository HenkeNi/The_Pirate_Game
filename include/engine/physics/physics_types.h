#pragma once

namespace cursed_engine
{
	constexpr uint16_t INVALID_WORLD_INDEX = 0;

	struct WorldId
	{
		uint16_t index = INVALID_WORLD_INDEX;
		uint16_t generation = 0;

		bool operator==(const WorldId&) const = default;

		static inline constexpr WorldId invalid()
		{
			return WorldId{ INVALID_WORLD_INDEX, 0 };
		}

		[[nodiscard]] inline constexpr bool isValid() const
		{
			return index != INVALID_WORLD_INDEX;
		}
	};
}