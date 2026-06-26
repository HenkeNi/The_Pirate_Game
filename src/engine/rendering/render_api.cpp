#include "engine/rendering/render_api.h"
#include "engine/rendering/renderer.h"

namespace cursed_engine
{
	RenderAPI::RenderAPI(Renderer* renderer)
		: m_renderer{ renderer }
	{
	}

	void RenderAPI::drawTexture(FRect rect, Texture& texture, Color color)
	{
		m_renderer->drawTexture(rect.x, rect.y, rect.w, rect.y, texture, color);
	}

	void RenderAPI::drawTexture(FVec2 pos, FVec2 size, Texture& texture, Color color)
	{
		m_renderer->drawTexture(pos.x, pos.y, size.x, size.y, texture, color);
	}

	void RenderAPI::drawTexture(float x, float y, float width, float height, Texture& texture, Color color)
	{
		m_renderer->drawTexture(x, y, width, height, texture, color);
	}

	void RenderAPI::drawGeometry(const Geometry& geometry, Texture& texture)
	{
		m_renderer->drawGeometry(geometry, texture);
	}

	void RenderAPI::drawOutlineRect(FRect rect, Color color)
	{
		m_renderer->drawOutlineRect(rect.x, rect.y, rect.w, rect.h, color);
	}

	void RenderAPI::drawOutlineRect(float x, float y, float w, float h, Color color)
	{
		m_renderer->drawOutlineRect(x, y, w, h, color);
	}

	void RenderAPI::drawFillRect(FRect rect, Color color)
	{
		m_renderer->drawFillRect(rect.x, rect.y, rect.w, rect.h, color);
	}

	void RenderAPI::drawFillRect(float x, float y, float w, float h, Color color)
	{
		m_renderer->drawFillRect(x, y, w, h, color);
	}

	void RenderAPI::drawLine(FVec2 start, FVec2 end, Color color)
	{
		m_renderer->drawLine(start.x, start.y, end.x, end.y, color);
	}

	void RenderAPI::drawLine(float startX, float startY, float endX, float endY, Color color)
	{
		m_renderer->drawLine(startX, startY, endX, endY, color);
	}

	void RenderAPI::drawText(Text& text, int x, int y)
	{
		m_renderer->drawText(text, x, y);
	}
}