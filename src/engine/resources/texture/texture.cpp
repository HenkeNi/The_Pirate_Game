#include "engine/resources/texture/texture.h"
#include <SDL3/SDL.h>
#include <cassert>

namespace cursed_engine
{
	SDLTexture::SDLTexture(SDL_Texture* texture)
		: m_texture{ texture }
	{
		assert(m_texture && "Invalid texture!");
		SDL_GetTextureSize(m_texture, &m_size.x, &m_size.y);
	}

	SDLTexture::~SDLTexture()
	{
		if (m_texture)
		{
			SDL_DestroyTexture(m_texture);
			m_texture = nullptr;
		}
	}

	SDLTexture::SDLTexture(SDLTexture&& other) noexcept
		: m_texture{ other.m_texture }, m_size{ other.m_size }
	{
		other.m_texture = nullptr;
	}

	SDLTexture& SDLTexture::operator=(SDLTexture&& other) noexcept
	{
		if (this != &other)
		{
			m_texture = other.m_texture;
			m_size = other.m_size;

			other.m_texture = nullptr;
		}

		return *this;
	}

	bool SDLTexture::isValid() const noexcept
	{
		return m_texture != nullptr;
	}
}