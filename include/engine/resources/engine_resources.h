#pragma once
#include "engine/core/subsystem.h"
#include "engine/resources/audio_manager.h"
#include "engine/resources/font_manager.h"
#include "engine/resources/texture_manager.h"
#include "engine/resources/text_manager.h"

namespace cursed_engine
{
	class Renderer;
	
	struct EngineResources : public Subsystem
	{
		explicit EngineResources(Renderer& renderer)
			: textManager{ textureManager, fontManager, renderer }
		{
		}

		AudioManager audioManager;
		FontManager fontManager;
		TextureManager textureManager;
		TextManager textManager;
	};
}