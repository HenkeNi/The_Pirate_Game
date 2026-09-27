#include "engine/physics/physics_debug_draw.h"
#include <box2d/box2d.h>

#include "engine/core/logger.h"
#include <format>

namespace cursed_engine
{
	b2DebugDraw g_debugDraw = b2DefaultDebugDraw(); // FIX THIS? - or make static / internal

	void drawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
	{
		RenderAPI* renderAPI = static_cast<RenderAPI*>(context);

		Geometry geometry;
		geometry.vertices.reserve(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			geometry.vertices.emplace_back(FVec2{ vertices[i].x, vertices[i].y }, FVec2{ 0.f, 0.f }, Color::orange);
		}

		geometry.indices.reserve(vertexCount);

		for (int i = 1; i < vertexCount - 1; ++i) 
		{
			geometry.indices.push_back(0);
			geometry.indices.push_back(i);
			geometry.indices.push_back(i + 1);
		}

		renderAPI->drawGeometry(geometry);		
	}

	void drawCircle(b2Vec2 center, float radius, b2HexColor color, void* context)
	{
		RenderAPI* renderAPI = static_cast<RenderAPI*>(context);

		renderAPI->drawFillCircle(center.x, center.y, radius, Color::orange);
	}

	void PhysicsDebugDraw::init(RenderAPI renderAPI)
	{
		m_renderAPI = renderAPI;
		m_worldId = WorldId::invalid();

		g_debugDraw.context = &m_renderAPI;
		
		g_debugDraw.DrawPolygonFcn = drawPolygon;
		g_debugDraw.DrawCircleFcn = drawCircle;

		g_debugDraw.drawShapes = true;
		g_debugDraw.drawBounds = true;
	}

	void PhysicsDebugDraw::draw()
	{			
		if (m_debugDrawEnabled && m_worldId.isValid())
		{
			b2World_Draw(b2WorldId{ m_worldId.index, m_worldId.generation }, &g_debugDraw);
		}

		
		//drawPolygonFcn();
	}

	void PhysicsDebugDraw::setDebugDrawEnabled(bool enabled)
	{
		m_debugDrawEnabled = enabled;
	}

	void PhysicsDebugDraw::setWorldId(WorldId worldId)
	{
		m_worldId = worldId;
	}
}