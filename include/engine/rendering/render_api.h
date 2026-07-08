#pragma once
#include "engine/rendering/render_types.h"
#include "engine/math/vec2.hpp"

namespace cursed_engine
{
	class Text;
	class Texture;
	class RenderBackend;

	// Consider having API push render commands to a queue instead...
	class RenderAPI
	{
	public:
		RenderAPI();
		RenderAPI(RenderBackend* backend);

		// rename to drawSprite?
		void drawTexture(FRect rect, Texture& texture, Color color = Color::white);
		void drawTexture(FVec2 pos, FVec2 size, Texture& texture, Color color = Color::white);
		void drawTexture(float x, float y, float width, float height, Texture& texture, Color color = Color::white);

		void drawGeometry(const Geometry& geometry, Texture& texture);

		void drawOutlineRect(FRect rect, Color color = Color::black);
		void drawOutlineRect(float x, float y, float w, float h, Color color = Color::black);

		void drawFillRect(FRect rect, Color color = Color::black);
		void drawFillRect(float x, float y, float w, float h, Color color = Color::black);

		void drawLine(FVec2 start, FVec2 end, Color color = Color::black);
		void drawLine(float startX, float startY, float endX, float endY, Color color = Color::black); // replace with Line struct?

		void drawText(Text& text, float x, float y);

	private:
		RenderBackend* m_backend;
	};
}