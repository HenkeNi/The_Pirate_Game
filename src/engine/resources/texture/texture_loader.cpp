#include "engine/resources/texture/texture_loader.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/texture/surface.h"
#include "engine/resources/resource_creator.h"
#include "engine/core/logger.h"
#include <SDL3_image/SDL_image.h>
#include <filesystem>

namespace cursed_engine
{
	TextureLoader::TextureLoader(ResourceCreator* creator)
		: m_creator{ creator }
	{ 
	}

	Texture TextureLoader::operator()(const TextureDescriptor& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			Logger::logError("Failed to load texture, invalid path: " + key.path); // stirng format!
			return Texture{ nullptr };
		}

		// use this if not modifying the texture:
		//auto* texture = IMG_LoadTexture(m_renderer.getRenderer(), path.string().c_str());
		auto* surface = IMG_Load(key.path.c_str()); // pass const char* instead?

		if (!surface)
		{
			Logger::logError("Unable to load image, path: " + key.path + ", error: " + SDL_GetError());
			return Texture{ nullptr };
		}

		Texture texture = m_creator->createTextureFromSurface(Surface{ surface });
		//SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer->getRenderer(), surface);

		return texture;
		/*if (!texture)
		{
			Logger::logError("Unable to create texture from surface, path: " + key.path + ", error: " + SDL_GetError());
			return Texture{ nullptr };
		}*/

		//return Texture{ texture };
	}
}