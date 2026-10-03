#include "engine/resources/resource_creators.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/font/font.h"
#include "engine/resources/audio/audio.h"
#include "engine/core/result.h"
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

	Result<AudioPtr> SDLAudioCreator::createAudio(const char* path) const
	{
		assert(m_mixer && "[SDLAudioCreator::createAudio] - Invalid Mixer!");

		bool predecode = false;
		MIX_Audio* audio = MIX_LoadAudio(m_mixer, path, predecode);

		if (!audio)
		{
			return Result<AudioPtr>::failure(std::format("Failed to create text! Reason: {}", SDL_GetError()));
		}

		return Result<AudioPtr>::success(std::make_unique<SDLAudio>(audio));
	}

	void SDLTextureCreator::init(SDL_Renderer* renderer)
	{
		m_renderer = renderer;
	}

	Result<TexturePtr> SDLTextureCreator::createTextureFromSurface(Surface surface) const
	{
		assert(m_renderer && "[SDLTextureCreator::createTextureFromSurface] - Invalid renderer!");

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface.surface);

		if (!texture)
		{
			return Result<TexturePtr>::failure(std::format("Failed to generate texture! Reason: {}", SDL_GetError()));
		}

		SDL_DestroySurface(surface.surface);

		return Result<TexturePtr>::success(std::make_unique<SDLTexture>(texture));
	}
}