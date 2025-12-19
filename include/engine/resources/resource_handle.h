#pragma once
#include <cstdint>

namespace cursed_engine
{
	template <typename Tag>
	struct ResourceHandle
	{
		uint32_t index;
		uint32_t version = 0;

		// TODO; add a isValid function that checks if index (or id) is numeric limits?

		bool isValid() const
		{
			return version >= 0 && index >= 0 && index < std::numeric_limits<uint32_t>::max(); // TODO; check!
		}

		bool operator==(const ResourceHandle& other) const
		{
			return index == other.index && version == other.version;
		}
	};

	//struct TextureTag {};
	//struct AudioTag {};

	//using TextureHandle = ResourceHandle<TextureTag>;
	//using AudioHandle = ResourceHandle<AudioTag>;
}