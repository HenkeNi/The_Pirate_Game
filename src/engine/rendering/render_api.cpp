#include "engine/rendering/render_api.h"
#include "engine/rendering/render_backend.h"

namespace cursed_engine
{
	RenderAPI::RenderAPI()
		: m_backend{ nullptr }
	{
	}

	RenderAPI::RenderAPI(RenderBackend* backend)
		: m_backend{ backend }
	{
	}

	void RenderAPI::drawTexture(FRect rect, Texture& texture, Color color)
	{
		m_backend->drawTexture(texture, rect, color);
	}

	void RenderAPI::drawTexture(FVec2 pos, FVec2 size, Texture& texture, Color color)
	{
		m_backend->drawTexture(texture, FRect{ pos.x, pos.y, size.x, size.y }, color);
	}

	void RenderAPI::drawTexture(float x, float y, float width, float height, Texture& texture, Color color)
	{
		m_backend->drawTexture(texture, FRect{ x, y, width, height }, color);
	}

	void RenderAPI::drawGeometry(const Geometry& geometry, Texture& texture)
	{
		m_backend->drawGeometry(geometry, &texture);
	}

	void RenderAPI::drawOutlineRect(FRect rect, Color color)
	{
		m_backend->drawOutlineRect(rect, color);
	}

	void RenderAPI::drawOutlineRect(float x, float y, float w, float h, Color color)
	{
		m_backend->drawOutlineRect(FRect{ x, y, w, h }, color);
	}

	void RenderAPI::drawFillRect(FRect rect, Color color)
	{
		m_backend->drawFillRect(rect, color);
	}

	void RenderAPI::drawFillRect(float x, float y, float w, float h, Color color)
	{
		m_backend->drawFillRect(FRect{ x, y, w, h }, color);
	}

	void RenderAPI::drawLine(FVec2 start, FVec2 end, Color color)
	{
		m_backend->drawLine(start, end, color);
	}

	void RenderAPI::drawLine(float startX, float startY, float endX, float endY, Color color)
	{
		m_backend->drawLine(FVec2{ startX, startY }, FVec2{ endX, endY }, color);
	}

	void RenderAPI::drawText(Text& text, float x, float y)
	{
		m_backend->drawText(text, FVec2{ x, y });
	}
}