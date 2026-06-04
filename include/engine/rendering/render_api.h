#pragma once
#include "engine/rendering/render_types.h"
#include "engine/math/vec2.hpp"

namespace cursed_engine
{
	class Text;
	class Texture;
	class Renderer;

	// Consider having API push render commands to a queue instead...
	class RenderAPI
	{
	public:
		RenderAPI(Renderer& renderer);

		void renderTexture(FRect rect, Texture& texture, Color color = Color::white);
		void renderTexture(FVec2 pos, FVec2 size, Texture& texture, Color color = Color::white);
		void renderTexture(float x, float y, float width, float height, Texture& texture, Color color = Color::white);

		void renderOutlineRect(FRect rect, Color color = Color::black);
		void renderOutlineRect(float x, float y, float w, float h, Color color = Color::black);

		void renderFillRect(FRect rect, Color color = Color::black);
		void renderFillRect(float x, float y, float w, float h, Color color = Color::black);

		void renderLine(FVec2 start, FVec2 end, Color color = Color::black);
		void renderLine(float startX, float startY, float endX, float endY, Color color = Color::black); // replace with Line struct?

		void renderText(Text& text, int x, int y);

	private:
		Renderer& m_renderer;
	};
}