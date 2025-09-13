#include "engine/rendering/renderer.h"
#include "engine/rendering/texture.h"
#include "engine/window/window.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <iostream>

namespace cursed_engine
{
	Renderer::Renderer()
		: m_renderer{ nullptr }
	{
	}

	bool Renderer::init(Window& window)
	{
		m_renderer = SDL_CreateRenderer(window.getWindow(), nullptr);

		if (!m_renderer)
		{
			SDL_Log("SDL could not create renderer!", SDL_GetError()); // TODO; replace with Logger::LogError
			return false;
		}

		return true;
	}

	void Renderer::shutdown()
	{
		SDL_DestroyRenderer(m_renderer);
	}

	void Renderer::clearScreen()
	{
		SDL_SetRenderDrawColor(m_renderer, 125, 125, 125, 255);
		SDL_RenderClear(m_renderer);
	}

	void Renderer::present()
	{
		SDL_RenderPresent(m_renderer);
	}

	void Renderer::renderTexture(float x, float y, Texture& texture)
	{
		SDL_FRect dstRect{ x, y, 25.f, 25.f };
		//SDL_FRect dstRect{ x, y, (float)texture.getWidth(), (float)texture.getHeight() };
		SDL_RenderTexture(m_renderer, texture.getTexture(), nullptr, &dstRect);
	}
}