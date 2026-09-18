#include "engine/physics/physics_debug_draw.h"
#include <box2d/box2d.h>

#include "engine/core/logger.h"

namespace cursed_engine
{
	b2DebugDraw g_debugDraw = b2DefaultDebugDraw(); // FIX THIS? - or make static / internal

	//void drawPolygonFcn(RenderAPI& renderAPI, const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
	void drawPolygonFcn(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
	{

		RenderAPI* renderAPI = static_cast<RenderAPI*>(context);

		Geometry geometry;
		geometry.vertices.reserve(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			//Vertex(FVec2 position, FVec2 uv, Color color = Color::white)
			geometry.vertices.emplace_back(FVec2{ vertices[i].x, vertices[i].y }, FVec2{ 0.f, 1.f });
		}

		renderAPI->drawFillRect(FRect{ vertices[0].x, vertices[0].y, vertices[0].x + 200.f, vertices[0].y + 200.f }, Color::pink);
		//renderAPI->drawOutlineRect(FRect{ vertices[0].x, vertices[0].y, vertices[0].x + 200.f, vertices[0].y + 200.f }, Color::pink);
		//renderAPI->drawGeometry(geometry, ); // allow drawing without texture?

		//renderAPI->drawFillRect(); // no drawRect or drawPolygon?

	}

	void PhysicsDebugDraw::init(RenderAPI renderAPI)
	{
		m_renderAPI = renderAPI;
		m_worldId = WorldId::invalid();

		g_debugDraw.context = &m_renderAPI;
		g_debugDraw.DrawPolygonFcn = drawPolygonFcn;

		g_debugDraw.drawShapes = true;
		g_debugDraw.drawBounds = true;
	}

	void PhysicsDebugDraw::draw()
	{
		//g_debugDraw.DrawPolygonFcn = [](const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
		//	{
		//		// m_renderAPI.drawLine();
		//	};
	
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