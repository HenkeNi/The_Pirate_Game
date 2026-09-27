#include "engine/resources/sdl_resource_loaders.h"
#include "engine/resources/audio/audio.h"
#include "engine/resources/resource_creator.h"
#include "engine/resources/text/font.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/texture/surface.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_audio.h>
#include <format>

namespace cursed_engine
{
	Result<Font> SDLFontLoader::operator()(const FontDescriptor& descriptor) const
	{
		if (!std::filesystem::exists(descriptor.path))
		{
			return Result<Font>::failure(std::format("Failed to load font! Invalid path: {}", descriptor.path));
		}

		TTF_Font* font = TTF_OpenFont(descriptor.path.c_str(), (float)descriptor.size);

		if (!font)
		{
			return Result<Font>::failure(std::format("Failed to load font. Reason {}", SDL_GetError()));
		}

		TTF_SetFontStyle(font, static_cast<TTF_FontStyleFlags>(descriptor.style));
		TTF_SetFontOutline(font, descriptor.outline);
		TTF_SetFontKerning(font, descriptor.kerning);

		return Result<Font>::success(Font{ font, descriptor });
	}

	Result<Audio> SDLAudioLoader::operator()(const AudioDescriptor& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			return Result<Audio>::failure(std::format("Failed to load Audio! Invalid path: {}", key.path));
		}

		SDL_AudioSpec spec;
		/*spec.freq = 48000;
		spec.format = SDL_AUDIO_F32;
		spec.channels = 2;*/

		Uint8* audioBuffer = nullptr;
		Uint32 audioLength = 0;

		if (!SDL_LoadWAV(key.path.c_str(), &spec, &audioBuffer, &audioLength))
		{
			return Result<Audio>::failure(std::format("Failed to load wav. Reason: {}", SDL_GetError()));
		}

		// Is this correct??
		auto* stream = SDL_CreateAudioStream(&spec, nullptr);
		if (!stream)
		{
			return Result<Audio>::failure(std::format("Failed to create audio stream. Reason: {}", SDL_GetError()));
		}

		return Result<Audio>::success(SDLAudio{ stream, audioBuffer, audioLength });
	}

	Result<Surface> SurfaceLoader::operator()(const std::filesystem::path& path) const
	{
		auto* surface = SDL_LoadBMP(path.string().c_str());
		if (!surface)
		{
			return Result<Surface>::failure(std::format("Failed to load window icon, path: {}", path.string()));
		}

		return Result<Surface>::success(Surface{ surface });
	}

	SDLTextureLoader::SDLTextureLoader(ResourceCreator* creator)
		: m_creator{ creator }
	{
	}

	Result<Texture> SDLTextureLoader::operator()(const TextureDescriptor& key) const
	{
		assert(m_creator && "Not a valid resource creator!");

		if (!std::filesystem::exists(key.path))
		{
			return Result<Texture>::failure(std::format("Failed to load texture, invalid path: {}", key.path));
		}

		// use this if not modifying the texture:
		//auto* texture = IMG_LoadTexture(m_renderer.getRenderer(), path.string().c_str());
		auto* surface = IMG_Load(key.path.c_str());

		if (!surface)
		{
			return Result<Texture>::failure(std::format("Unable to load image! Path: {}. Error: ", key.path, SDL_GetError()));
		}

		return m_creator->createTextureFromSurface(Surface{ surface });
	}
}