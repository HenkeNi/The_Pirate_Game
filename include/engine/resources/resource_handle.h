#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

namespace cursed_engine
{
	template <typename Tag>
	struct ResourceHandle
	{
		uint32_t index = std::numeric_limits<uint32_t>::max();
		uint32_t generation = 0;

		static constexpr ResourceHandle invalid()
		{
			return ResourceHandle(std::numeric_limits<uint32_t>::max(), 0);
		}

		bool isValid() const
		{
			return generation > 0 && (index >= 0 && index < std::numeric_limits<uint32_t>::max()); // TODO; check!
		}

		bool operator==(const ResourceHandle& other) const
		{
			return generation == other.generation && index == other.index;
		}
	};
}