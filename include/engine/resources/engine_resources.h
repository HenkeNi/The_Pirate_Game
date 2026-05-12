#pragma once
#include "engine/core/subsystem.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/font.h"
#include "engine/audio/audio.h"
//#include "engine/resources/audio_manager.h"
//#include "engine/resources/font_manager.h"
//#include "engine/resources/texture_manager.h"

#include "engine/resources/text_manager.h"
#include "engine/rendering/text_factory.h"

#include "engine/rendering/text_renderer.h" // fix! remove this dpeency (and perhaps TextFactory)

namespace cursed_engine
{
	class Renderer;
	
	// TODO; make class? ResourceRegistry?
	struct EngineResources : public Subsystem
	{
		explicit EngineResources(const ResourceConfig& resourceConfig, Renderer& renderer, TextRenderer& textRenderer)
			: textureManager{ resourceConfig, TextureLoader{ renderer } }, 
			audioManager{ resourceConfig, AudioLoader{} },
			fontManager{ resourceConfig, FontLoader{} },
			textManager{ fontManager, renderer },
			textFactory{ fontManager, textRenderer.getTextEngine() }
		{
		}

		void update(uint64_t currentFrame, float deltaTime) 
		{
			audioManager.update(currentFrame, deltaTime);
			fontManager.update(currentFrame, deltaTime);
			textureManager.update(currentFrame, deltaTime);
		}

		AudioManager audioManager; // AudioAssecor?
		FontManager fontManager;
		TextureManager textureManager;
		TextManager textManager;

		TextFactory textFactory; // here?
	};
}