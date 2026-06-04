#include "engine/rendering/render_api.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/text_renderer.h"

namespace cursed_engine
{
	RenderAPI::RenderAPI(Renderer& renderer)
		: m_renderer{ renderer }
	{
	}

	void RenderAPI::renderTexture(FRect rect, Texture& texture, Color color)
	{
		m_renderer.renderTexture(rect.x, rect.y, rect.w, rect.y, texture, color);
	}

	void RenderAPI::renderTexture(FVec2 pos, FVec2 size, Texture& texture, Color color)
	{
		m_renderer.renderTexture(pos.x, pos.y, size.x, size.y, texture, color);
	}

	void RenderAPI::renderTexture(float x, float y, float width, float height, Texture& texture, Color color)
	{
		m_renderer.renderTexture(x, y, width, height, texture, color);
	}

	void RenderAPI::renderOutlineRect(FRect rect, Color color)
	{
		m_renderer.renderOutlineRect(rect.x, rect.y, rect.w, rect.h, color);
	}

	void RenderAPI::renderOutlineRect(float x, float y, float w, float h, Color color)
	{
		m_renderer.renderOutlineRect(x, y, w, h, color);
	}

	void RenderAPI::renderFillRect(FRect rect, Color color)
	{
		m_renderer.renderFillRect(rect.x, rect.y, rect.w, rect.h, color);
	}

	void RenderAPI::renderFillRect(float x, float y, float w, float h, Color color)
	{
		m_renderer.renderFillRect(x, y, w, h, color);
	}

	void RenderAPI::renderLine(FVec2 start, FVec2 end, Color color)
	{
		m_renderer.renderLine(start.x, start.y, end.x, end.y, color);
	}

	void RenderAPI::renderLine(float startX, float startY, float endX, float endY, Color color)
	{
		m_renderer.renderLine(startX, startY, endX, endY, color);
	}

	void RenderAPI::renderText(Text& text, int x, int y)
	{
		m_renderer.renderText(text, x, y);
	}
}