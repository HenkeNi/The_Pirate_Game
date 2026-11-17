#include "engine/resources/resources.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	Resources::Resources()
		: m_audioManager{ nullptr }, m_fontManager{ nullptr }, m_textureManager{ nullptr }
	{
	}

	Resources::Resources(AudioManager* audioManager, FontManager* fontManager, TextureManager* textureManager)
		: m_audioManager{ audioManager }, m_fontManager{ fontManager }, m_textureManager{ textureManager }
	{
	}
}