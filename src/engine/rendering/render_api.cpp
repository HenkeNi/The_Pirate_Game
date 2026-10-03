#include "engine/rendering/render_api.h"
#include "engine/rendering/render_backend.h"
#include "engine/resources/texture/texture.h"

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

	void RenderAPI::setRenderState(RenderState state)
	{
		m_backend->setRenderState(std::move(state));
	}

	void RenderAPI::drawTexture(const Texture& texture, FRect dst, Color color)
	{
		FRect src{ 0, 0, texture.getWidth(), texture.getHeight() };

		m_backend->drawTexture(texture, std::move(src), dst, color);
	}

	void RenderAPI::drawTexture(const Texture& texture, FVec2 pos, FVec2 size, Color color)
	{
		FRect src{ 0, 0, texture.getWidth(), texture.getHeight() };
		FRect dst{ pos.x, pos.y, size.x, size.y };

		m_backend->drawTexture(texture, std::move(src), std::move(dst), color);
	}

	void RenderAPI::drawTexture(const Texture& texture, FRect src, FRect dst, Color color)
	{
		m_backend->drawTexture(texture, src, dst, color);
	}

	void RenderAPI::drawTexture(const Texture& texture, FVec2 srcPos, FVec2 srcSize, FVec2 dstPos, FVec2 dstSize, Color color)
	{
		FRect src{ srcPos.x, srcPos.y, srcSize.x, srcSize.y };
		FRect dst{ dstPos.x, dstPos.y, dstSize.x, dstSize.y };

		m_backend->drawTexture(texture, std::move(src), std::move(dst), color);
	}

	void RenderAPI::drawGeometry(const Geometry& geometry, const Texture& texture)
	{
		m_backend->drawGeometry(geometry, &texture);
	}

	void RenderAPI::drawGeometry(const Geometry& geometry)
	{
		m_backend->drawGeometry(geometry);
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

	void RenderAPI::drawOutlineCircle(FVec2 pos, float radius, Color color)
	{
		m_backend->drawOutlineCircle(pos, radius, color);
	}

	void RenderAPI::drawOutlineCircle(float x, float y, float radius, Color color)
	{
		m_backend->drawOutlineCircle(FVec2{ x, y }, radius, color);
	}

	void RenderAPI::drawFillCircle(FVec2 pos, float radius, Color color)
	{
		m_backend->drawFillCircle(pos, radius, color);
	}

	void RenderAPI::drawFillCircle(float x, float y, float radius, Color color)
	{
		m_backend->drawFillCircle(FVec2{ x, y }, radius, color);
	}

	void RenderAPI::drawLine(FVec2 start, FVec2 end, Color color)
	{
		m_backend->drawLine(start, end, color);
	}

	void RenderAPI::drawLine(float startX, float startY, float endX, float endY, Color color)
	{
		m_backend->drawLine(FVec2{ startX, startY }, FVec2{ endX, endY }, color);
	}

	void RenderAPI::drawText(const Text& text, float x, float y)
	{
		m_backend->drawText(text, FVec2{ x, y });
	}
}