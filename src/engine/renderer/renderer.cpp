#include "engine/renderer/renderer.h"
#include "engine/window/window.h"
#include <SDL3/SDL_render.h>
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
			std::cerr << "Error creating renderer!\n";
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
		SDL_RenderClear(m_renderer);
	}

	void Renderer::present()
	{
		SDL_RenderPresent(m_renderer);
	}
}