#include "engine/physics/physics.h"
#include <box2d/box2d.h>

namespace cursed_engine
{
	PhysicsWorld::PhysicsWorld(uint16_t index, uint16_t generation)
		: m_index{ index }, m_generation{ generation }
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
		destroy();
	}

	PhysicsWorld::PhysicsWorld(PhysicsWorld&& other) noexcept
		: m_index{ other.m_index }, m_generation{ other.m_generation }
	{
		other.m_index = 0;
		other.m_generation = 0;
	}

	PhysicsWorld& PhysicsWorld::operator=(PhysicsWorld&& other) noexcept
	{
		m_index = other.m_index;
		m_generation = other.m_generation;

		other.m_index = 0;
		other.m_generation = 0;

		return *this;
	}


	PhysicsBody PhysicsWorld::createBody(PhysicsBody::BodyType type)
	{
		b2BodyDef bodyDef;
		bodyDef.type = (b2BodyType)type;

		b2BodyId bodyId = b2CreateBody({ m_index, m_generation }, &bodyDef);

		return PhysicsBody{ bodyId.index1, bodyId.world0, bodyId.generation }; 
	}

	void PhysicsWorld::destroyBody(PhysicsBody body)
	{
		b2DestroyBody({ body.index, body.world, body.generation });
	}

	void PhysicsWorld::destroy()
	{
		b2DestroyWorld({ m_index, m_generation });
	}

	//bool Physics::isValidBody(PhysicsBody body) const
	//{
	//	return b2Body_IsValid({ body.index, body.world, body.generation });
	//}






	PhysicsWorld Physics::createWorld() const
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.enableSleep = true;
		worldDef.gravity = { 0.f, 0.f };


		b2WorldId id = b2CreateWorld(&worldDef);
		
		PhysicsWorld worldID{ id.index1, id.generation };

 		return worldID;
	}

	/*bool Physics::isValidWorld(PhysicsWorld world) const
	{
		return b2World_IsValid({ world.index, world.generation });
	}*/
}