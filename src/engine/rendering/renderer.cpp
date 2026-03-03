#include "engine/rendering/renderer.h"
#include "engine/rendering/texture.h"
#include "engine/rendering/render_types.h"
#include "engine/window/window.h"
#include "engine/core/logger.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace cursed_engine
{
	Renderer::Renderer()
		: m_renderer{ nullptr }
	{
	}

	bool Renderer::init(Window& window)
	{
		m_renderer = SDL_CreateRenderer(window.getHandle(), nullptr);

		if (!m_renderer)
		{
			Logger::logError(std::string("Failed to create renderer: ") + SDL_GetError());
			return false;
		}

		if (!TTF_Init())
		{
			Logger::logError(std::string("Failed to init TTF: ") + SDL_GetError());
		}

		return true;
	}

	void Renderer::shutdown()
	{
		SDL_DestroyRenderer(m_renderer);
		TTF_Quit();
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

	void Renderer::renderTexture(FRect rect, Texture& texture)
	{
		assert(false && "Not implemented!");
	}

	void Renderer::renderTexture(FVec2 pos, FVec2 size, Texture& texture)
	{
		renderTexture(pos.x, pos.y, size.x, size.y, texture);
	}

	void Renderer::renderTexture(float x, float y, float width, float height, Texture& texture)
	{
		SDL_FRect dstRect{ x, y, width, height };

		//SDL_FRect dstRect{ x, y, (float)texture.getWidth(), (float)texture.getHeight() };
		SDL_RenderTexture(m_renderer, texture.getTexture(), nullptr, &dstRect);
	}

	void Renderer::renderOutlineRects(std::span<FRect> rects)
	{
		/*m_sdlRectBuffer.clear();
		m_sdlRectBuffer.reserve(rects.size());

		for (const auto& r : rects)
		{
			m_sdlRectBuffer.push_back(SDL_FRect{
				r.x, r.y, r.w, r.h
				});
		}

		SDL_RenderRects(m_renderer, m_sdlRectBuffer.data(),
			static_cast<int>(m_sdlRectBuffer.size()));	*/	
	
		static_assert(sizeof(FRect) == sizeof(SDL_FRect));
		static_assert(std::is_standard_layout_v<FRect>);
		static_assert(std::is_standard_layout_v<SDL_FRect>);

		SDL_RenderRects(m_renderer, reinterpret_cast<const SDL_FRect*>(rects.data()), 
			static_cast<int>(rects.size()));
	}

	void Renderer::renderOutlineRect(FRect rect, Color color)
	{
		renderOutlineRect(rect.x, rect.y, rect.w, rect.h, std::move(color));
	}

	void Renderer::renderOutlineRect(float x, float y, float w, float h, Color color)
	{
		const SDL_FRect sdlRect{ x, y, w, h };
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a); // TODO; reset color before rendering textures?
		SDL_RenderRect(m_renderer, &sdlRect);
	}

	void Renderer::renderFillRect(FRect rect, Color color)
	{
		renderFillRect(rect.x, rect.y, rect.w, rect.h, std::move(color));
	}

	void Renderer::renderFillRect(float x, float y, float w, float h, Color color)
	{
		const SDL_FRect sdlRect{ x, y, w, h };
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer, &sdlRect);
	}

	void Renderer::renderLine(FVec2 start, FVec2 end, Color color)
	{
		renderLine(start.x, start.y, end.x, end.y, color);
	}

	void Renderer::renderLine(float startX, float startY, float endX, float endY, Color color)
	{
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a); // TODO; reset color before rendering textures
		SDL_RenderLine(m_renderer, startX, startY, endX, endY);
	}
}