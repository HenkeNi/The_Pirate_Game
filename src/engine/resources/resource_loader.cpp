#include "engine/resources/resource_loader.h"
#include "engine/core/logger.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/renderer.h"
#include "engine/audio/audio.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_audio.h>

namespace cursed_engine
{
	TextureLoader::TextureLoader(Renderer& renderer)
		: m_renderer{ renderer }
	{
	}

	std::unique_ptr<Texture> TextureLoader::load(const std::filesystem::path& path) const
	{
		if (!std::filesystem::exists(path))
		{
			Logger::logError("Failed to load texture, invalid path: " + path.string());
			return nullptr;
		}

		auto* surface = IMG_Load(path.string().c_str()); // pass const char* instead?

		if (!surface)
		{
			Logger::logError("Unable to load image, path: " + path.string() + ", error: " + SDL_GetError());
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer.getRenderer(), surface);

		if (!texture)
		{
			Logger::logError("Unable to create texture from surface, path: " + path.string() + ", error: " + SDL_GetError());
			return nullptr;
		}

		SDL_DestroySurface(surface);
		return std::make_unique<Texture>(texture);
	}

	std::unique_ptr<Audio> AudioLoader::load(const std::filesystem::path& path) const
	{
		if (!std::filesystem::exists(path))
		{
			Logger::logError("Failed to load audio, invalid path: " + path.string());
			return nullptr;
		}

		SDL_AudioSpec spec;
		/*spec.freq = 48000;
		spec.format = SDL_AUDIO_F32;
		spec.channels = 2;*/

		Uint8* audioBuffer = nullptr;
		Uint32 audioLength = 0;

		if (!SDL_LoadWAV(path.string().c_str(), &spec, &audioBuffer, &audioLength))
		{
			Logger::logError("Failed to load audio, invalid path: " + path.string());
			return nullptr;
		}

		// Is this correct??
		auto* stream = SDL_CreateAudioStream(&spec, nullptr);
		if (!stream)
		{
			Logger::logError(std::format("Failed to create audio stream! Error: {}", SDL_GetError()).c_str());
			return nullptr;
		}

		return std::make_unique<Audio>(stream, audioBuffer, audioLength);
	}
}