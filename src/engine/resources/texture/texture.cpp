#include "engine/resources/texture/texture.h"
#include "engine/core/logger.h"
#include <SDL3/SDL.h>
#include <format>

namespace cursed_engine
{
	Texture::Texture()
		: m_texture{ nullptr }, m_width{ 0 }, m_height{ 0 }
	{
	}

	Texture::Texture(SDL_Texture* texture)
		: m_texture{ texture }, m_width{ 0 }, m_height{ 0 }
	{
		if (m_texture && !SDL_GetTextureSize(m_texture, &m_width, &m_height))
		{
			Logger::logError(std::format("Texture initialized with invalid texture{}", SDL_GetError()));
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
}