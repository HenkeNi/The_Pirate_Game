#pragma once
#include "engine/math/vec2.hpp"
#include "engine/math/rect.hpp"
#include "engine/resources/resource_creators.h"
#include "engine/rendering/text/text_creator.h"
#include "engine/rendering/render_types.h"
#include <SDL3/SDL.h>
#include <vector>

// [Consider] - replacing std::vector<SDL_Vertex> with SDL_Vertex* (raw array) (no need for SDL3 include)
// [Consider] - passing Window as pointer (easily to pass null if not used by backend, OR pass a context struct of some sort)

// TODO; add vsync (toggling)!!!!!!!!!!!! (moved from window)...

struct TTF_TextEngine;
//struct SDL_Renderer; - not needed since including sdl.h?! (for vertex buffer)

namespace cursed_engine
{
	class Texture;
	class Text;
	class Window;
	
	template <typename T>
	class Result;

	// TODO; rename Renderer?

	class RenderBackend final
	{
	public:
		RenderBackend();
		~RenderBackend() = default;

		RenderBackend(const RenderBackend&) = delete;
		RenderBackend(RenderBackend&&) = delete;

		RenderBackend& operator=(const RenderBackend&) = delete;
		RenderBackend& operator=(RenderBackend&&) = delete;

		[[nodiscard]] Result<void> init(Window& window);
		void shutdown();

		void beginFrame();
		void endFrame();

		void setRenderState(RenderState state);
		bool setVSync(bool enable);

		[[nodiscard]] const RenderStatistics* getStatistics() const noexcept;

		[[nodiscard]] const TextureCreator* getTextureCreator() const noexcept;
		[[nodiscard]] const TextCreator* getTextCreator() const noexcept;

		void drawTexture(const Texture& texture, FRect src, FRect dst, Color mod);
		void drawGeometry(const Geometry& geometry, const Texture* texture = nullptr);

		void drawOutlineRect(FRect dst, Color color);
		void drawFillRect(FRect dst, Color color);

		void drawOutlineCircle(FVec2 pos, float radius, Color color);
		void drawFillCircle(FVec2 pos, float radius, Color color);

		void drawLine(FVec2 start, FVec2 end, Color color);
		void drawText(const Text& text, FVec2 pos);

	private:
		void populateVertexBuffer(const Geometry& geometry);

		struct ResourceCreators
		{
			TextureCreator textureCreator;
			TextCreator textCreator;
		};

		std::vector<SDL_Vertex> m_vertexBuffer;
		ResourceCreators m_resourceCreators;

		RenderStatistics m_statistics;
		RenderState m_renderState;

		TTF_TextEngine* m_textEngine;
		SDL_Renderer* m_renderer;
	};
}