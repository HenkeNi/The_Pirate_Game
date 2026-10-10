#include "engine/physics/physics_debug_draw.h"
#include "engine/rendering/text/text_creator.h"
#include "engine/rendering/text/text.h"
#include <box2d/box2d.h>

#include "engine/core/logger.h"
#include <format>

// [TODO] - maybe this requires threadsafe render backend???
// [TODO] - drawSolidPolygon should handle rotation in transform!?

namespace
{
	using namespace cursed_engine;

	FVec2 toFVec2(b2Vec2 b2Vec2)
	{
		return FVec2{ b2Vec2.x, b2Vec2.y };
	}

	void drawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
	{
		PhysicsDebugDrawContext* drawContext = static_cast<PhysicsDebugDrawContext*>(context);
		
		for (int i = 0; i < vertexCount; ++i)
		{
			FVec2 p1 = toFVec2(vertices[i]);
			FVec2 p2 = toFVec2(vertices[(i + 1) % vertexCount]);

			drawContext->renderAPI.drawLine(p1, p2);
		}		
	}

	void drawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context)
	{
		PhysicsDebugDrawContext* drawContext = static_cast<PhysicsDebugDrawContext*>(context);

		Geometry geometry;
		geometry.vertices.reserve(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			geometry.vertices.emplace_back(toFVec2(vertices[i] + transform.p), FVec2{ 0.f, 0.f }, Color::orange);
		}

		geometry.indices.reserve(vertexCount);

		for (int i = 1; i < vertexCount - 1; ++i)
		{
			geometry.indices.push_back(0);
			geometry.indices.push_back(i);
			geometry.indices.push_back(i + 1);
		}

		drawContext->renderAPI.drawGeometry(geometry);
	}

	void drawCircle(b2Vec2 center, float radius, b2HexColor color, void* context)
	{
		PhysicsDebugDrawContext* drawContext = static_cast<PhysicsDebugDrawContext*>(context);
		drawContext->renderAPI.drawOutlineCircle(center.x, center.y, radius, Color::orange);
	}

	void drawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context)
	{
		PhysicsDebugDrawContext* drawContext = static_cast<PhysicsDebugDrawContext*>(context);
		drawContext->renderAPI.drawFillCircle(transform.p.x, transform.p.y, radius, Color::orange);
	}
	
	void drawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void* context)
	{
		PhysicsDebugDrawContext* drawContext = static_cast<PhysicsDebugDrawContext*>(context);

		drawContext->renderAPI.drawFillCircle(p1.x, p1.y, radius, Color::orange);
		drawContext->renderAPI.drawFillRect(FRect{ p1.x - radius, p1.y, p2.x + radius, p2.y });
		drawContext->renderAPI.drawFillCircle(p2.x, p2.y, radius, Color::orange);
	}
	
	void drawText(b2Vec2 p, const char* s, b2HexColor color, void* context)
	{
		PhysicsDebugDrawContext* drawContext = static_cast<PhysicsDebugDrawContext*>(context);

		// Dont create text every time?
		auto fontHandle = drawContext->fontManager->getHandleById("treasure.regular", FontStyle::Normal, 40, 0, 0);

		if (fontHandle.isValid())
		{
			const Font& font = drawContext->fontManager->get(fontHandle);
			Result<TextPtr> result = drawContext->textCreator->createText(s, font);
			
			if (result.ok())
			{
				TextPtr text = result.take();
				text->setTextColor(Color::black);
				drawContext->renderAPI.drawText(*text, p.x, p.y);
			}
		}
	}
}

namespace cursed_engine
{
	b2DebugDraw g_debugDraw = b2DefaultDebugDraw(); // FIX THIS? - or make static / internal

	void PhysicsDebugDraw::init(PhysicsDebugDrawContext context)
	{
		m_drawContext = std::move(context);
		//m_renderAPI = renderAPI;
		m_worldId = WorldId::invalid();

		g_debugDraw.context = &m_drawContext;
		
		g_debugDraw.DrawPolygonFcn = drawPolygon;
		g_debugDraw.DrawSolidPolygonFcn = drawSolidPolygon;
		g_debugDraw.DrawCircleFcn = drawCircle;
		g_debugDraw.DrawSolidCircleFcn = drawSolidCircle;
		g_debugDraw.DrawStringFcn = drawText;
		g_debugDraw.DrawSolidCapsuleFcn = drawSolidCapsule;
		//g_debugDraw.DrawSegmentFcn

		g_debugDraw.drawShapes = true;
		g_debugDraw.drawBounds = true;
		g_debugDraw.drawBodyNames = true;
	}

	void PhysicsDebugDraw::draw()
	{			
		if (m_enabled && m_worldId.isValid())
		{
			b2World_Draw(b2WorldId{ m_worldId.index, m_worldId.generation }, &g_debugDraw);
		}

		
		//drawPolygonFcn();
	}

	void PhysicsDebugDraw::setEnabled(bool enabled)
	{
		m_enabled = enabled;
	}

	void PhysicsDebugDraw::setWorldId(WorldId worldId)
	{
		m_worldId = worldId;
	}
}