#include "engine/rendering/render_backend.h"
#include "engine/resources/texture/texture.h"
#include "engine/resources/text/text.h"
#include "engine/platform/window.h"
#include "engine/core/result.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <numbers>

namespace
{
	constexpr int CIRCLE_SEGMENTS = 32;

	SDL_FRect toSDLRect(float x, float y, float w, float h)
	{
		return SDL_FRect{ x, y, w, h };
	}

	SDL_FRect toSDLRect(const cursed_engine::FRect& rect)
	{
		return toSDLRect(rect.x, rect.y, rect.w, rect.h);
	}

	SDL_FColor toSDLColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		constexpr float inv = 1.0f / 255.f;

		return SDL_FColor{
			r * inv,
			g * inv,
			b * inv,
			a * inv,
		};
	}

	SDL_FColor toSDLColor(const cursed_engine::Color& color)
	{
		return toSDLColor(color.r, color.g, color.b, color.a);
	}

	SDL_FPoint toSDLPoint(const cursed_engine::FVec2& v)
	{
		return SDL_FPoint(v.x, v.y);
	}

	SDL_FPoint toSDLPoint(float x, float y)
	{
		return SDL_FPoint(x, y);
	}

	SDL_Vertex toSDLVertex(const cursed_engine::FVec2& position, const cursed_engine::FVec2& uv, const cursed_engine::Color& color)
	{
		return SDL_Vertex{
			SDL_FPoint{ position.x, position.y },
			SDL_FColor{ toSDLColor(color) },
			SDL_FPoint{ uv.x, uv.y }
		};
	}

	SDL_Vertex toSDLVertex(const cursed_engine::Vertex& vertex)
	{
		return toSDLVertex(vertex.position, vertex.uv, vertex.color);
	}

	cursed_engine::FVec2 worldToScreen(const cursed_engine::FVec2& position, const cursed_engine::Projection& proj, const cursed_engine::View& view)
	{
		// TODO; just position or width and height as well?

		// screen = world - camera...
		return cursed_engine::FVec2{
			position.x - view.position.x,
			position.y - view.position.y
		};

		//rect.x = (dst.x - view.position.x); // * view.zoom + (projection.size.x * 0.5f);
		//rect.y = (dst.y - view.position.y); // * view.zoom + (projection.size.y * 0.5f);
	}
}

namespace cursed_engine
{
	SDLRenderBackend::SDLRenderBackend()
		: m_renderer{ nullptr }, m_textEngine{ nullptr }, m_statistics{}
	{
	}

	Result<void> SDLRenderBackend::init(Window& window)
	{
		auto* nativeHandle = window.getNativeHandle();
		if (!nativeHandle)
		{
			return Result<void>::failure("Invalid native window handle");
		}

		SDL_Window* sdlWindow = static_cast<SDL_Window*>(nativeHandle); // or cast? or pass handle direclty in function?

		m_renderer = SDL_CreateRenderer(sdlWindow, nullptr);

		if (!m_renderer)
		{
			return Result<void>::failure(std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
		}

		m_textEngine = TTF_CreateRendererTextEngine(m_renderer);

		if (!m_textEngine)
		{
			return Result<void>::failure(std::format("TTF_CreateRendererTextEngine failed: {}", SDL_GetError()));
		}

		m_resourceCreator.init(m_textEngine, m_renderer);

		return Result<void>::success();
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

	void SDLRenderBackend::setRenderState(RenderState state)
	{
		m_renderState = std::move(state);
	}

	void SDLRenderBackend::drawTexture(Texture& texture, FRect src, FRect dst, Color mod)
	{
		const FVec2 screenPosition = worldToScreen(FVec2{ dst.x, dst.y }, m_renderState.projection, m_renderState.view);

 		const SDL_FRect dstRect = toSDLRect(screenPosition.x, screenPosition.y, dst.w, dst.h);
		const SDL_FRect srcRect = toSDLRect(src.x, src.y, src.w, src.h);

		SDL_SetTextureColorMod(texture.getTexture(), mod.r, mod.g, mod.b);
		SDL_SetTextureAlphaMod(texture.getTexture(), mod.a);
		SDL_RenderTexture(m_renderer, texture.getTexture(), &srcRect, &dstRect);
	}

	void SDLRenderBackend::drawGeometry(const Geometry& geometry, Texture* texture)
	{
		populateVertexBuffer(geometry);
		SDL_RenderGeometry(m_renderer, texture ? texture->getTexture() : nullptr, m_vertexBuffer.data(), m_vertexBuffer.size(), geometry.indices.data(), geometry.indices.size());
	}

	void SDLRenderBackend::drawOutlineRect(FRect dst, Color color)
	{
		const FVec2 screenPosition = worldToScreen(FVec2{ dst.x, dst.y }, m_renderState.projection, m_renderState.view);
		const SDL_FRect rect = toSDLRect(screenPosition.x, screenPosition.y, dst.w, dst.h);

		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderRect(m_renderer, &rect);
	}

	void SDLRenderBackend::drawFillRect(FRect dst, Color color)
	{
		const FVec2 screenPosition = worldToScreen(FVec2{ dst.x, dst.y }, m_renderState.projection, m_renderState.view);
		const SDL_FRect rect = toSDLRect(screenPosition.x, screenPosition.y, dst.w, dst.h);

		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(m_renderer, &rect);
	}

	void SDLRenderBackend::drawOutlineCircle(FVec2 pos, float radius, Color color)
	{
		SDL_FPoint points[CIRCLE_SEGMENTS + 1];

		FVec2 worldPosition = worldToScreen(pos, m_renderState.projection, m_renderState.view);

		for (int i = 0; i < CIRCLE_SEGMENTS; ++i)
		{
			const float theta = (2.0f * std::numbers::pi_v<float> * i) / CIRCLE_SEGMENTS;
			points[i].x = worldPosition.x + std::cos(theta) * radius;
			points[i].y = worldPosition.y + std::sin(theta) * radius;
		}
		
		points[CIRCLE_SEGMENTS] = points[0];

		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderLines(m_renderer, points, CIRCLE_SEGMENTS + 1);
	}

	void SDLRenderBackend::drawFillCircle(FVec2 pos, float radius, Color color)
	{
		FVec2 worldPosition = worldToScreen(pos, m_renderState.projection, m_renderState.view);
		SDL_FColor sdlColor = toSDLColor(color);

		constexpr int vertexCount = CIRCLE_SEGMENTS + 2;
		SDL_Vertex vertices[vertexCount];

		// Center vertex		
		vertices[0].position = toSDLPoint(worldPosition);
		vertices[0].color = toSDLColor(color);
		vertices[0].tex_coord = { 0.f, 0.f };

		for (int i = 0; i <= CIRCLE_SEGMENTS; ++i)
		{
			const float theta = (2.0f * std::numbers::pi_v<float> * i) / CIRCLE_SEGMENTS;

			SDL_Vertex& v = vertices[i + 1];
			v.position.x = worldPosition.x + std::cos(theta) * radius;
			v.position.y = worldPosition.y + std::sin(theta) * radius;
			v.color = sdlColor;
			v.tex_coord = { 0.f, 0.f };
		}

		constexpr int indexCount = CIRCLE_SEGMENTS * 3;
		int indices[indexCount];

		for (int i = 0; i < CIRCLE_SEGMENTS; ++i)
		{
			indices[i * 3 + 0] = 0;
			indices[i * 3 + 1] = i + 1;
			indices[i * 3 + 2] = i + 2;
		}

		SDL_RenderGeometry(m_renderer, nullptr, vertices, vertexCount, indices, indexCount);
	}

	void SDLRenderBackend::drawLine(FVec2 start, FVec2 end, Color color)
	{
		const FVec2 screenStart = worldToScreen(FVec2{ start.x, start.y }, m_renderState.projection, m_renderState.view);
		const FVec2 screenEnd = worldToScreen(FVec2{ end.x, end.y }, m_renderState.projection, m_renderState.view);

		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
		SDL_RenderLine(m_renderer, screenStart.x, screenStart.y, screenEnd.x, screenEnd.y);
	}

	void SDLRenderBackend::drawText(Text& text, FVec2 pos)
	{
		assert(text.isValid() && "SDLRenderBackend::drawText - Invalid text found!");

		const FVec2 screenPosition = worldToScreen(pos, m_renderState.projection, m_renderState.view);

		TTF_DrawRendererText(text.get(), screenPosition.x, screenPosition.y);
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
				const FVec2 screenPosition = worldToScreen(FVec2{ vertex.position.x, vertex.position.y }, m_renderState.projection, m_renderState.view);
				m_vertexBuffer.push_back(toSDLVertex(screenPosition, vertex.uv, vertex.color));
			});
	}
}