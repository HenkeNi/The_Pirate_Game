#include "engine/resources/resource_loaders.h"
#include "engine/resources/audio/audio.h"
#include "engine/resources/resource_creators.h"
#include "engine/resources/text/font.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/texture/surface.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_audio.h>
#include <format>

namespace cursed_engine
{
	SDLAudioLoader::SDLAudioLoader(const AudioCreator* creator)
		: m_creator{ creator }
	{
	}

	Result<std::unique_ptr<Audio>> SDLAudioLoader::operator()(const AudioDescriptor& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			return Result<std::unique_ptr<Audio>>::failure(std::format("Failed to load Audio! Invalid path: {}", key.path));
		}

		return m_creator->createAudio(key.path.c_str());

		//if (!SDL_LoadWAV(key.path.c_str(), &spec, &audioBuffer, &audioLength))
		//{
		//	return Result<Audio>::failure(std::format("Failed to load wav. Reason: {}", SDL_GetError()));
		//}

		// Is this correct??
		/*auto* stream = SDL_CreateAudioStream(&spec, nullptr);
		if (!stream)
		{
			return Result<Audio>::failure(std::format("Failed to create audio stream. Reason: {}", SDL_GetError()));
		}

		return Result<Audio>::success(SDLAudio{ stream, audioBuffer, audioLength });*/
	}

	Result<std::unique_ptr<Font>> SDLFontLoader::operator()(const FontDescriptor& descriptor) const
	{
		if (!std::filesystem::exists(descriptor.path))
		{
			return Result<std::unique_ptr<Font>>::failure(std::format("Failed to load font! Invalid path: {}", descriptor.path));
		}

		TTF_Font* font = TTF_OpenFont(descriptor.path.c_str(), (float)descriptor.size);

		if (!font)
		{
			return Result<std::unique_ptr<Font>>::failure(std::format("Failed to load font. Reason {}", SDL_GetError()));
		}

		TTF_SetFontStyle(font, static_cast<TTF_FontStyleFlags>(descriptor.style)); // 
		TTF_SetFontOutline(font, descriptor.outline);
		TTF_SetFontKerning(font, descriptor.kerning);

		return Result<std::unique_ptr<Font>>::success(std::make_unique<Font>(font, descriptor));
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

	SDLTextureLoader::SDLTextureLoader(const TextureCreator* creator)
		: m_creator{ creator }
	{
	}

	Result<std::unique_ptr<Texture>> SDLTextureLoader::operator()(const TextureDescriptor& key) const
	{
		assert(m_creator && "Not a valid resource creator!");

		if (!std::filesystem::exists(key.path))
		{
			return Result<std::unique_ptr<Texture>>::failure(std::format("Failed to load texture, invalid path: {}", key.path));
		}

		// use this if not modifying the texture:
		//auto* texture = IMG_LoadTexture(m_renderer.getRenderer(), path.string().c_str());
		auto* surface = IMG_Load(key.path.c_str());

		if (!surface)
		{
			return Result<std::unique_ptr<Texture>>::failure(std::format("Unable to load image! Path: {}. Error: ", key.path, SDL_GetError()));
		}

		return m_creator->createTextureFromSurface(Surface{ surface });
	}
}