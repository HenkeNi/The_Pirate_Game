#pragma once
#include "engine/core/subsystem.h"
#include "engine/math/vec2.h" // TODO; put in pch...
#include "engine/rendering/render_types.h"
#include <span>

// TODO; handle metadata?? objects rendered, etc...

struct SDL_Renderer;

namespace cursed_engine
{
	class Window;
	class Texture;
	struct FRect;
	//struct Color;

	class Renderer : public Subsystem
	{
	public:
		Renderer();

		bool init(Window& window);
		void shutdown();

		void clearScreen();
		void present(); // rename?

		// are these safe to mark const?
		void renderTexture(FRect rect, Texture& texture);
		void renderTexture(FVec2 pos, FVec2 size, Texture& texture);
		void renderTexture(float x, float y, float width, float height, Texture& texture);

		// TODO; add renderRects

		// renderFillRect
		// renderOutlineRect`?

		// renderFillRects
		// renderLines
		// point points

		void renderOutlineRects(std::span<FRect> rects); // renderRectsBashed? use rects??? color?

		void renderOutlineRect(FRect rect, Color color = Color::black);
		void renderOutlineRect(float x, float y, float w, float h, Color color = Color::black);

		void renderFillRect(FRect rect, Color color = Color::black);
		void renderFillRect(float x, float y, float w, float h, Color color = Color::black);

		void renderLine(FVec2 start, FVec2 end, Color color = Color::black);
		void renderLine(float startX, float startY, float endX, float endY, Color color = Color::black); // replace with Line struct?

		[[nodiscard]] inline SDL_Renderer* getRenderer() noexcept { return m_renderer; }

	private:
		SDL_Renderer* m_renderer;
	};
}