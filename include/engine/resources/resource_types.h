#pragma once
#include <memory>

namespace cursed_engine
{
	class Audio;
	using AudioPtr = std::unique_ptr<Audio>;

	class Font;
	using FontPtr = std::unique_ptr<Font>;

	class Texture;
	using TexturePtr = std::unique_ptr<Texture>;
}