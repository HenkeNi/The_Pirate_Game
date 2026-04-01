#pragma once
#include "engine/rendering/font.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	struct FontKey
	{
		std::string fontId;
		int fontSize;

		bool operator==(const FontKey& other) const noexcept
		{
			return fontId == other.fontId && fontSize == other.fontSize;
		}
	};

	struct FontKeyHasher
	{
		std::size_t operator()(const FontKey& key) const noexcept
		{
			std::size_t h = std::hash<std::string>{}(key.fontId);

			h ^= std::hash<int>{}(key.fontSize)
				+ 0x9e3779b9 + (h << 6) + (h >> 2);

			return h;
			//return std::hash<std::string>{}(key.fontId) ^ std::hash<std::size_t>{}(key.fontSize);
		}
	};

	using FontManager = ResourceManager<Font, FontKey, FontKeyHasher>;
}