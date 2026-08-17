#pragma once
#include "engine/math/vec2.hpp"
#include "engine/math/rect.hpp"
#include "engine/resources/resource_creator.h"
#include "engine/rendering/render_types.h"
#include "engine/utils/non_copyable.h"
#include <SDL3/SDL.h>
#include <vector>

// [Consider] - replacing std::vector<SDL_Vertex> with SDL_Vertex* (raw array) (no need for SDL3 include)
// [Consider] - making RenderBackend inherit from TextureFactory?
// [Consider] - init function to instead accept a void* (nativeWindowHandle)?

struct TTF_TextEngine;
//struct SDL_Renderer; - not needed since including sdl.h?!

namespace cursed_engine
{
	class Texture;
	class Text;
	class Window;
	struct Result;

#pragma region Render_Backend

	class RenderBackend : public NonCopyable
	{
	public:
		RenderBackend() = default;
		virtual ~RenderBackend() = default;

		RenderBackend(RenderBackend&&) = delete;
		RenderBackend& operator=(RenderBackend&&) = delete;

		[[nodiscard]] virtual Result init(Window& window) = 0;
		virtual void shutdown() = 0;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual void setRenderState(RenderState state) = 0;

		[[nodiscard]] virtual const RenderStatistics* getStatistics() const noexcept = 0;
		[[nodiscard]] virtual ResourceCreator* getResourceCreator() noexcept = 0;

		virtual void drawTexture(Texture& texture, FRect src, FRect dst, Color mod) = 0;
		virtual void drawGeometry(const Geometry& geometry, Texture* texture = nullptr) = 0; // or pass Texture&

		virtual void drawOutlineRect(FRect dst, Color color) = 0;
		virtual	void drawFillRect(FRect dst, Color color) = 0;

		virtual	void drawLine(FVec2 start, FVec2 end, Color color) = 0;
		virtual void drawText(Text& text, FVec2 pos) = 0;
	};

#pragma endregion

#pragma region SDL_Render_Device

	class SDLRenderBackend : public RenderBackend
	{
	public:
		SDLRenderBackend();
		~SDLRenderBackend() = default;

		[[nodiscard]] Result init(Window& window) override;
		void shutdown() override;

		void beginFrame() override;
		void endFrame() override;

		void setRenderState(RenderState state) override;

		[[nodiscard]] const RenderStatistics* getStatistics() const noexcept override;
		[[nodiscard]] ResourceCreator* getResourceCreator() noexcept override;

		void drawTexture(Texture& texture, FRect src, FRect dst, Color mod) override;
		void drawGeometry(const Geometry& geometry, Texture* texture = nullptr) override;

		void drawOutlineRect(FRect dst, Color color) override;
		void drawFillRect(FRect dst, Color color) override;

		void drawLine(FVec2 start, FVec2 end, Color color) override;
		void drawText(Text& text, FVec2 pos) override;

	private:
		void populateVertexBuffer(const Geometry& geometry);

		std::vector<SDL_Vertex> m_vertexBuffer;
		SDLResourceCreator m_resourceCreator;

		RenderStatistics m_statistics;
		RenderState m_renderState;

		TTF_TextEngine* m_textEngine;
		SDL_Renderer* m_renderer;
	};

#pragma endregion
}