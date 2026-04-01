#pragma once
#include "engine/audio/audio.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	struct AudioKey
	{
		std::string id;

		bool operator==(const AudioKey& other) const noexcept
		{
			return id == other.id;
		}
	};

	struct AudioKeyHasher
	{
		std::size_t operator()(const AudioKey& key) const noexcept
		{
			return std::hash<std::string>{}(key.id);
		}
	};

	using AudioManager = ResourceManager<Audio, AudioKey, AudioKeyHasher>;
}