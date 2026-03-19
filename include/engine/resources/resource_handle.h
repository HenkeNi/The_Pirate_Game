#pragma once
#include <cstdint>

namespace cursed_engine
{
	template <typename Tag>
	struct ResourceHandle
	{
		// TODO, add constexpr constructor

		uint32_t index;
		uint32_t version = 0;

		static constexpr ResourceHandle invalid()
		{
			return ResourceHandle(std::numeric_limits<uint32_t>::max(), 0);
		}

		//static const ResourceHandle invalid;

		bool isValid() const
		{
			return version >= 0 && index >= 0 && index < std::numeric_limits<uint32_t>::max(); // TODO; check!
		}

		bool operator==(const ResourceHandle& other) const
		{
			return index == other.index && version == other.version;
		}
	};

	//template <typename Tag>
	//const ResourceHandle<Tag> ResourceHandle<Tag>::invalid{ -1, 0 };

	//template <typename Tag> 
	//constexpr ResourceHandle<Tag> INVALID_RESOURCE_HANDLE{ -1, 0 }; // or static function?

	//struct TextureTag {};
	//struct AudioTag {};

	//using TextureHandle = ResourceHandle<TextureTag>;
	//using AudioHandle = ResourceHandle<AudioTag>;
}