#include "engine/physics/physics.h"
#include "engine/physics/physics_debug_draw.h"
#include <box2d/box2d.h>

namespace cursed_engine
{
#pragma region Helpers

	b2BodyType tob2BodyType(ColliderType type)
	{
		switch (type)
		{
		case ColliderType::Dynamic:
			return b2_dynamicBody;
		case ColliderType::Static:
			return b2_staticBody;
		case ColliderType::Kinematic:
			return b2_kinematicBody;
		}

		assert(false && "Unhandled ColliderType in toBox2DBodyType");
		std::abort();
	}

	b2Vec2 tob2Vec(const FVec2& vec2)
	{
		return b2Vec2{ vec2.x, vec2.y };
	}

	b2Polygon tob2Polygon(const Shape& shape)
	{
		switch (shape.type)
		{
		case Shape::ShapeType::Square:
			return b2MakeSquare(shape.data.Square.halfExtent);
		case Shape::ShapeType::Rectangle:
			return b2MakeBox(shape.data.Rectangle.width * 0.5f, shape.data.Rectangle.height * 0.5f);
			//case cursed_engine::Shape::Circle:
				//return b2MakeRoundedBox()		
				//return b2MakeAABB();
		}

		assert(false && "Unhandled ShapeType in tob2Polygon");
		std::abort();
	}

	b2WorldId tob2WorldId(const WorldId& id)
	{
		return b2WorldId{ id.index, id.generation };
	}

	b2BodyId tob2BodyId(const PhysicsBody::BodyId& id)
	{
		return b2BodyId{ id.index, id.world, id.generation };
	}

	FVec2 toFVec2(const b2Vec2& vec)
	{
		return FVec2{ vec.x, vec.y };
	}

#pragma endregion

	void PhysicsBody::setLinearVelocity(const FVec2& velocity)
	{
		b2Body_SetLinearVelocity(tob2BodyId(bodyId), tob2Vec(velocity));
	}

	FVec2 PhysicsBody::getPosition() const noexcept
	{
		return toFVec2(b2Body_GetPosition(tob2BodyId(bodyId)));
	}

	PhysicsWorld::PhysicsWorld(float gravityX, float gravityY)
		: PhysicsWorld(FVec2{ gravityX, gravityY })
	{
	}

	PhysicsWorld::PhysicsWorld(FVec2 gravity)
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.enableSleep = true;
		worldDef.gravity = tob2Vec(gravity);

		//worldDef.workerCount = 4;
		//worldDef.enqueueTask = myAddTaskFunction;
		//worldDef.finishTask = myFinishTaskFunction;
		//worldDef.userTaskContext = &myTaskSystem;

		b2WorldId worldId = b2CreateWorld(&worldDef);

		m_worldId.index = worldId.index1;
		m_worldId.generation = worldId.generation;
	}

	PhysicsWorld::~PhysicsWorld()
	{
		const b2WorldId id = tob2WorldId(m_worldId);

		if (B2_IS_NULL(id))
			return;

		b2DestroyWorld(id);
	}

	PhysicsWorld::PhysicsWorld(PhysicsWorld&& other) noexcept
		: m_worldId{ other.m_worldId }
	{
		const b2WorldId nullId = b2_nullWorldId;
		other.m_worldId = { nullId.index1, nullId.generation };
	}

	PhysicsWorld& PhysicsWorld::operator=(PhysicsWorld&& other) noexcept
	{
		if (this == &other)
			return *this;

		if (m_worldId.isValid())
		{
			b2DestroyWorld(tob2WorldId(m_worldId));
		}

		m_worldId = other.m_worldId;
		other.m_worldId = WorldId::invalid();

		return *this;
	}

	void PhysicsWorld::step()
	{
		// need to create bodies before step/update

		float timeStep = 1.0f / 60.0f;
		int subStepCount = 4;

		b2World_Step(tob2WorldId(m_worldId), timeStep, subStepCount);
	}

	PhysicsBody PhysicsWorld::createBody(const BodyDefinition& definition)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = tob2BodyType(definition.type);
		bodyDef.position = tob2Vec(definition.position);

		b2BodyId bodyId = b2CreateBody(tob2WorldId(m_worldId), &bodyDef);

		b2Polygon polygon = tob2Polygon(definition.shape);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.material.friction = 0.3f;

		b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &polygon);

		// do something with id?

		return PhysicsBody{ bodyId.index1, bodyId.world0, bodyId.generation };
	}

	PhysicsBody PhysicsWorld::createGroundBody(const FVec2& position)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.position = static_cast<b2Vec2>(0.0f, -10.0f);

		b2BodyId bodyId = b2CreateBody(tob2WorldId(m_worldId), &bodyDef);

		// decide what shape to create => pass in size! (union?)
		b2Polygon polygon = b2MakeBox(50.f, 10.f);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		b2CreatePolygonShape(bodyId, &shapeDef, &polygon);

		return PhysicsBody{ bodyId.index1, bodyId.world0, bodyId.generation };  // return this?
	}

	//PhysicsBody PhysicsWorld::createDynamicBody(FVec2 position, ColliderType type, Shape shape)
	//{
	//	b2BodyDef bodyDef;
	//	bodyDef.type = b2BodyType::b2_dynamicBody; // or use parameter?
	//	bodyDef.position.x = position.x;
	//	bodyDef.position.y = position.y; // o local helper function??

	//	b2BodyId bodyId = b2CreateBody({ m_index, m_generation }, &bodyDef);

	//	b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);

	//	b2ShapeDef shapeDef = b2DefaultShapeDef();
	//	shapeDef.density = 1.0f; // default...
	//	shapeDef.material.friction = 0.3f;

	//	b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

	//	return PhysicsBody{ bodyId.index1, bodyId.world0, bodyId.generation };
	//}

	/*PhysicsBody PhysicsWorld::createStaticBody()
	{

		return PhysicsBody{};
	}*/

	/*PhysicsBody PhysicsWorld::createKinematicBody()
	{

		return PhysicsBody{};
	}*/

	void PhysicsWorld::destroyBody(PhysicsBody body)
	{
		b2DestroyBody({ body.bodyId.index, body.bodyId.world, body.bodyId.generation });
	}


	//bool Physics::isValidBody(PhysicsBody body) const
	//{
	//	return b2Body_IsValid({ body.index, body.world, body.generation });
	//}


	/*bool Physics::isValidWorld(PhysicsWorld world) const
	{
		return b2World_IsValid({ world.index, world.generation });
	}*/

	PhysicsAPI::PhysicsAPI(PhysicsDebugDraw* physicsDebugDraw)
		: m_physicsDebugDraw{ physicsDebugDraw }
	{
	}

	void PhysicsAPI::setDebugDrawEnabled(bool enabled)
	{
		m_physicsDebugDraw->setDebugDrawEnabled(enabled);
	}

	void PhysicsAPI::setWorldId(WorldId worldId)
	{
		m_physicsDebugDraw->setWorldId(worldId);
	}

	constexpr bool PhysicsAPI::isDebugDrawEnabled() const noexcept
	{
		return m_physicsDebugDraw->isDebugDrawEnabled();
	}
}