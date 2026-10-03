#pragma once
#include "engine/math/vec2.hpp"
#include "engine/math/rect.hpp"
#include "engine/rendering/render_types.h"

namespace cursed_engine
{
	// [Consider] - having API push render commands to a queue instead...
	// [Consider] - renaming RenderFacade?

	class Text;
	class Texture;
	class RenderBackend;

	class RenderAPI
	{
	public:
		RenderAPI();
		RenderAPI(RenderBackend* backend);

		void setRenderState(RenderState state);

		void drawTexture(const Texture& texture, FRect dst, Color color = Color::white);
		void drawTexture(const Texture& texture, FVec2 pos, FVec2 size, Color color = Color::white);

		void drawTexture(const Texture& texture, FRect src, FRect dst, Color color = Color::white);
		void drawTexture(const Texture& texture, FVec2 srcPos, FVec2 srcSize, FVec2 dstPos, FVec2 dstSize, Color color = Color::white);

		void drawGeometry(const Geometry& geometry, const Texture& texture);
		void drawGeometry(const Geometry& geometry);

		void drawOutlineRect(FRect rect, Color color = Color::black);
		void drawOutlineRect(float x, float y, float w, float h, Color color = Color::black);

		void drawFillRect(FRect rect, Color color = Color::black);
		void drawFillRect(float x, float y, float w, float h, Color color = Color::black);

		void drawOutlineCircle(FVec2 pos, float radius, Color color = Color::black);
		void drawOutlineCircle(float x, float y, float radius, Color color = Color::black);

		void drawFillCircle(FVec2 pos, float radius, Color color = Color::black);
		void drawFillCircle(float x, float y, float radius, Color color = Color::black);

		void drawLine(FVec2 start, FVec2 end, Color color = Color::black);
		void drawLine(float startX, float startY, float endX, float endY, Color color = Color::black); // replace with Line struct?

		void drawText(const Text& text, float x, float y);

	private:
		RenderBackend* m_backend;
	};
}