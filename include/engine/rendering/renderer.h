#pragma once
#include "engine/rendering/render_types.h"
#include "engine/math/vec2.hpp" // TODO; put in pch...
#include <span>

struct SDL_Renderer;
struct TTF_TextEngine;

namespace cursed_engine
{
	class Window;
	class Texture;
	struct FRect;
	class Text;
	
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		//virtual void beginFrame() = 0;
		//virtual void endFrame() = 0;

	};

	// Rename SDLREnderer?
	class Renderer : public IRenderer
	{
	public:
		Renderer();

		bool init(Window& window); // window capabilities instead?
		void shutdown();

		void clearScreen();
		void present(); // rename?

		// are these safe to mark const?
		void renderTexture(float x, float y, float width, float height, Texture& texture, Color color = Color::white);
		void renderOutlineRects(std::span<FRect> rects); // renderRectsBashed? use rects??? color?
		void renderOutlineRect(float x, float y, float w, float h, Color color = Color::black);
		void renderFillRect(float x, float y, float w, float h, Color color = Color::black);
		void renderLine(float startX, float startY, float endX, float endY, Color color = Color::black); // replace with Line struct?
		void renderDebugText(float x, float y, const char* str);
		void renderText(Text& text, int x, int y);
		// renderFillRect
		// renderFillRects
		// renderLines
		// point points

		[[nodiscard]] inline SDL_Renderer* getRenderer() noexcept { return m_renderer; }
		[[nodiscard]] inline TTF_TextEngine* getTextEngine() noexcept { return m_textEngine; }

	private:
		SDL_Renderer* m_renderer;
		TTF_TextEngine* m_textEngine;
	};
}