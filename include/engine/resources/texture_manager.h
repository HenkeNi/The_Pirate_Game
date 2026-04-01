#pragma once
#include "engine/rendering/texture.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	struct TextureKey
	{
		std::string id;

		bool operator==(const TextureKey& other) const noexcept
		{
			return id == other.id;
		}
	};

	struct TextureKeyHasher
	{
		std::size_t operator()(const TextureKey& key) const noexcept
		{
			return std::hash<std::string>{}(key.id);
		}
	};

	using TextureManager = ResourceManager<Texture, TextureKey, TextureKeyHasher>;
}