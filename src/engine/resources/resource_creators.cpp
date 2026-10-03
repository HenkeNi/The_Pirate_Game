#include "engine/resources/resource_creators.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/text.h"
#include "engine/resources/text/font.h"
#include "engine/resources/audio/audio.h"
#include "engine/core/result.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cassert>
#include <format>

namespace cursed_engine
{
	void SDLAudioCreator::init(MIX_Mixer* mixer)
	{
		m_mixer = mixer;
	}

	Result<std::unique_ptr<Audio>> SDLAudioCreator::createAudio(const char* path) const
	{
		assert(m_mixer && "[SDLAudioCreator::createAudio] - Invalid Mixer!");

		bool predecode = false;
		MIX_Audio* audio = MIX_LoadAudio(m_mixer, path, predecode);

		if (!audio)
		{
			return Result<std::unique_ptr<Audio>>::failure(std::format("Failed to create text! Reason: {}", SDL_GetError()));
		}

		return Result<std::unique_ptr<Audio>>::success(std::make_unique<SDLAudio>(audio));
	}

	void SDLTextCreator::init(TTF_TextEngine* textEngine)
	{
		m_textEngine = textEngine;
	}

	Result<Text> SDLTextCreator::createText(const std::string& text, Font& font) const
	{
		assert(m_textEngine && "[SDLTextCreator::createText] - Invalid text engine!");

		TTF_Text* textObject = TTF_CreateText(m_textEngine, font.getInternal(), text.c_str(), text.size());

		if (!textObject)
		{
			return Result<Text>::failure(std::format("Failed to create text! Reason: {}", SDL_GetError()));
		}

		return Result<Text>::success(textObject);
	}

	void SDLTextureCreator::init(SDL_Renderer* renderer)
	{
		m_renderer = renderer;
	}

	Result<std::unique_ptr<Texture>> SDLTextureCreator::createTextureFromSurface(Surface surface) const
	{
		assert(m_renderer && "[SDLTextureCreator::createTextureFromSurface] - Invalid renderer!");

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface.surface);

		if (!texture)
		{
			return Result<std::unique_ptr<Texture>>::failure(std::format("Failed to generate texture! Reason: {}", SDL_GetError()));
		}

		SDL_DestroySurface(surface.surface);

		return Result<std::unique_ptr<Texture>>::success(std::make_unique<SDLTexture>(texture));
	}
}