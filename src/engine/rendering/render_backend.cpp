#include "engine/rendering/render_backend.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/text.h"
#include "engine/platform/window.h"
#include "engine/core/result.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>

namespace cursed_engine
{
#pragma region Helpers

	SDL_FRect toSDLRect(const FRect& rect)
	{
		return SDL_FRect{ rect.x, rect.y, rect.w, rect.h };
	}

	SDL_FColor toSDLColor(const Color& color)
	{
		constexpr float inv = 1.0f / 255.f;

		return SDL_FColor{
			color.r * inv,
			color.g * inv,
			color.b * inv,
			color.a * inv,
		};
	}

	SDL_Vertex toSDLVertex(const Vertex& vertex)
	{
		return SDL_Vertex{
			SDL_FPoint{ vertex.position.x, vertex.position.y },
			SDL_FColor{ toSDLColor(vertex.color) },
			SDL_FPoint{ vertex.uv.x, vertex.uv.y }
		};
	}

#pragma endregion

	SDLRenderBackend::SDLRenderBackend()
		: m_renderer{ nullptr }, m_textEngine{ nullptr }, m_statistics{}
	{
	}

	Result SDLRenderBackend::init(Window& window)
	{
		auto* nativeHandle = window.getNativeHandle();
		if (!nativeHandle)
		{
			return Result::failure("Invalid native window handle");
		}

		SDL_Window* sdlWindow = static_cast<SDL_Window*>(nativeHandle); // or cast? or pass handle direclty in function?

		m_renderer = SDL_CreateRenderer(sdlWindow, nullptr);

		if (!m_renderer)
		{
			return Result::failure(std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
		}

		m_textEngine = TTF_CreateRendererTextEngine(m_renderer);

		if (!m_textEngine)
		{
			return Result::failure(std::format("TTF_CreateRendererTextEngine failed: {}", SDL_GetError()));
		}

		m_resourceCreator.init(m_textEngine, m_renderer);

		return Result::success();
	}

	void SDLRenderBackend::shutdown()
	{
		SDL_DestroyRenderer(m_renderer);
		TTF_DestroyRendererTextEngine(m_textEngine);
	}

	void SDLRenderBackend::beginFrame()
	{
		m_statistics = RenderStatistics{ 0 };

		SDL_SetRenderDrawColor(m_renderer, 125, 125, 125, 255);
		SDL_RenderClear(m_renderer);
	}

	void SDLRenderBackend::SDLRenderBackend::endFrame()
	{
		SDL_RenderPresent(m_renderer);
	}

	void SDLRenderBackend::drawTexture(Texture& texture, const FRect& dst, Color mod)
	{
		SDL_SetTextureColorMod(texture.getTexture(), mod.r, mod.g, mod.b);
		
		SDL_FRect rect = toSDLRect(dst);
		SDL_RenderTexture(m_renderer, texture.getTexture(), nullptr, &rect);
	}

	void SDLRenderBackend::drawGeometry(const Geometry& geometry, Texture* texture)
	{
		populateVertexBuffer(geometry);
		SDL_RenderGeometry(m_renderer, texture ? texture->getTexture() : nullptr, m_vertexBuffer.data(), m_vertexBuffer.size(), geometry.indices.data(), geometry.indices.size());
	}

	void SDLRenderBackend::drawOutlineRect(const FRect& dst, Color color)
	{
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

		SDL_FRect rect = toSDLRect(dst);
		SDL_RenderRect(m_renderer, &rect);
	}

	void SDLRenderBackend::drawFillRect(const FRect& dst, Color color)
	{
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

		SDL_FRect rect = toSDLRect(dst);
		SDL_RenderFillRect(m_renderer, &rect);
	}

	void SDLRenderBackend::drawLine(const FVec2& start, const FVec2& end, Color color)
	{
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
	}

	void SDLRenderBackend::drawText(Text& text, const FVec2& pos)
	{
		assert(text.isValid() && "Draw Text; not a valid text!");
		TTF_DrawRendererText(text.get(), pos.x, pos.y);
	}

	const RenderStatistics* SDLRenderBackend::getStatistics() const noexcept
	{
		return &m_statistics;
	}

	ResourceCreator* SDLRenderBackend::getResourceCreator() noexcept
	{
		return &m_resourceCreator;
	}

	void SDLRenderBackend::populateVertexBuffer(const Geometry& geometry)
	{
		m_vertexBuffer.clear();
		m_vertexBuffer.reserve(geometry.vertices.size());

		std::for_each(geometry.vertices.begin(), geometry.vertices.end(),
			[&](const Vertex& vertex)
			{
				m_vertexBuffer.push_back(toSDLVertex(vertex));
			});
	}
}