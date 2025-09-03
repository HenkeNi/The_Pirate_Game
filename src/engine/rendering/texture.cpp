#include "engine/rendering/texture.h"
#include <SDL3/SDL.h>

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
		destroy();
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

	void Texture::init(SDL_Texture* texture)
	{
		if (m_texture = texture; !SDL_GetTextureSize(m_texture, &m_width, &m_height))
		{
			SDL_Log("SDL_GetTextureSize failed: %s", SDL_GetError());
		}
	}

	void Texture::destroy()
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = m_height = 0;
	}

	bool Texture::isLoaded() const
	{
		return false;
	}
}