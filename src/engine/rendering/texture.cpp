#include "engine/rendering/texture.h"
#include "engine/rendering/renderer.h"
#include "engine/core/logger.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace cursed_engine
{
	Texture::Texture(SDL_Texture* texture)
		: m_texture{ texture }, m_width{ 0 }, m_height{ 0 }
	{
		if (m_texture && !SDL_GetTextureSize(m_texture, &m_width, &m_height))
		{
			SDL_Log("SDL_GetTextureSize failed: %s", SDL_GetError());
		}
	}

	Texture::~Texture()
	{
		release();
	}

	Texture::Texture(Texture&& other) noexcept
		: m_texture{ other.m_texture }, m_width{ other.m_width }, m_height{ other.m_height }
	{
		other.m_texture = nullptr;
	}


	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			m_texture = other.m_texture;
			m_width = other.m_width;
			m_height = other.m_height;

			other.m_texture = nullptr;
		}

		return *this;
	}

	bool Texture::isLoaded() const
	{
		return m_texture != nullptr;
	}

	void Texture::release()
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = m_height = 0;
	}



	TextureLoader::TextureLoader(Renderer& renderer)
		: m_renderer{ renderer }
	{
	}

	Texture TextureLoader::operator()(const TextureKey& key) const
	{
		if (!std::filesystem::exists(key.path))
		{
			Logger::logError("Failed to load texture, invalid path: " + key.path); // stirng format!
			return Texture{};
		}

		// use this if not modifying the texture:
		//auto* texture = IMG_LoadTexture(m_renderer.getRenderer(), path.string().c_str());
		auto* surface = IMG_Load(key.path.c_str()); // pass const char* instead?

		if (!surface)
		{
			Logger::logError("Unable to load image, path: " + key.path + ", error: " + SDL_GetError());
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer.getRenderer(), surface);

		if (!texture)
		{
			Logger::logError("Unable to create texture from surface, path: " + key.path + ", error: " + SDL_GetError());
			return Texture{};
		}

		SDL_DestroySurface(surface);
		return Texture{ texture };
	}
}