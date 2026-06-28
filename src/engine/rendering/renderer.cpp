#include "engine/rendering/renderer.h"
#include "engine/platform/window/window.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/text.h"
#include "engine/core/logger.h"
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <cassert>
#include <format>

namespace cursed_engine
{
	Renderer::Renderer()
		: m_renderer{ nullptr }, m_textEngine{ nullptr }, m_statistics{ 0 }
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
			Logger::logError(std::format("Failed to initialize TTF! Error: {}", SDL_GetError()));
			return false;
		}

		m_textEngine = TTF_CreateRendererTextEngine(m_renderer);

		if (!m_textEngine)
		{
			Logger::logError(std::format("Failed to create a TextEngine. Error: {}", SDL_GetError()));
			return false;
		}

		return true;
	}

	void Renderer::shutdown()
	{
		SDL_DestroyRenderer(m_renderer);

		TTF_DestroyRendererTextEngine(m_textEngine);
		TTF_Quit();

		m_textEngine = nullptr;
	}

	void Renderer::beginFrame()
	{
		m_statistics = RenderStatistics{ 0 };

		SDL_SetRenderDrawColor(m_renderer, 125, 125, 125, 255);
		SDL_RenderClear(m_renderer);
	}

	void Renderer::endFrame()
	{
		SDL_RenderPresent(m_renderer);
	}

	void Renderer::drawTexture(float x, float y, float width, float height, Texture& texture, Color color)
	{
		SDL_FRect dstRect{ x, y, width, height };

		SDL_SetTextureColorMod(texture.getTexture(), color.r, color.g, color.b);

		//SDL_FRect dstRect{ x, y, (float)texture.getWidth(), (float)texture.getHeight() };
		SDL_RenderTexture(m_renderer, texture.getTexture(), nullptr, &dstRect);
	}

	void Renderer::drawOutlineRects(std::span<FRect> rects)
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

	void Renderer::drawOutlineRect(float x, float y, float w, float h, Color color)
	{
		const SDL_FRect sdlRect{ x, y, w, h };
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a); // TODO; reset color before rendering textures?
		SDL_RenderRect(m_renderer, &sdlRect);
	}

	void Renderer::drawFillRect(float x, float y, float w, float h, Color color)
	{
		const SDL_FRect sdlRect{ x, y, w, h };
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer, &sdlRect);
	}

	void Renderer::drawLine(float startX, float startY, float endX, float endY, Color color)
	{
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a); // TODO; reset color before rendering textures
		SDL_RenderLine(m_renderer, startX, startY, endX, endY);
	}

	void Renderer::drawDebugText(float x, float y, const char* str)
	{
		SDL_SetRenderDrawColor(m_renderer, Color::white.r, Color::white.g, Color::white.b, Color::white.a);
		SDL_RenderDebugText(m_renderer, x, y, str);
	}

	void Renderer::drawText(Text& text, int x, int y)
	{
		assert(text.isValid() && "Not a valid text");
		TTF_DrawRendererText(text.get(), x, y);
	}

	void Renderer::drawGeometry(const Geometry& geometry, Texture& texture)
	{
		std::vector<SDL_Vertex> vertices;
		vertices.reserve(geometry.vertices.size());

		// copy vertices - find better way?
		std::for_each(geometry.vertices.begin(), geometry.vertices.end(),
			[&](const Vertex& vertex)
			{
				// COLOR need to be converted to range from 0-1...
				vertices.emplace_back(
					SDL_FPoint{ vertex.position.x, vertex.position.y },
					SDL_FColor{ (float)vertex.color.r / 255.f, (float)vertex.color.g / 255.f, (float)vertex.color.b / 255.f, (float)vertex.color.a / 255.f },
					SDL_FPoint{ vertex.uv.x, vertex.uv.y }); // maybe wont work?! - convert types
			});

		// SDL_Vertex[geometry.vertices.size()] vertices;

		/*constexpr std::vector<int> i_indices{ 0, 1, 2,
										  2, 1, 3 };*/

		const auto& indices = geometry.indices;

		SDL_RenderGeometry(m_renderer, texture.getTexture(), vertices.data(), vertices.size(), indices.data(), indices.size());
	}

	//void Renderer::renderVertices(Texture& texture, const Vertex* vertices, int numberOfVertices, const int* indices, int numberOfIndices)
	//{
	//	SDL_RenderGeometry(m_renderer, texture.getTexture(), vertices, numberOfIndices, indices, numberOfIndices);
	//}
}