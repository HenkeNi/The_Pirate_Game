#pragma once
#include "engine/resources/resource_manager.hpp" // instead???????????

// TEST
#include "engine/resources/text/font.h"
#include "engine/resources/text/font_loader.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/texture/texture_loader.h"
#include "engine/resources/audio/audio.h"


namespace cursed_engine
{
	//class Texture;
	//struct TextureDescriptor;
	//struct TextureLoader;
	//struct Audio;
	//struct AudioDescriptor;
	//struct AudioLoader;

	/*template <typename Resource, typename Descriptor, typename Loader>
	class ResourceManager;*/

	//class Font;
	//struct FontDescriptor;
	//struct FontLoader;
	//using FontManager = ResourceManager<class Font, struct FontDescriptor, struct FontLoader>;
	using FontManager = ResourceManager<Font, FontDescriptor, FontLoader>;

	using TextureManager = ResourceManager<Texture, TextureDescriptor, TextureLoader>;
	using AudioManager = ResourceManager<Audio, AudioDescriptor, AudioLoader>;
}