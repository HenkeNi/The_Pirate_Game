#include "engine/physics/physics.h"
#include "engine/core/logger.h"
#include <box2d/box2d.h>
#include <format>

namespace cursed_engine
{
	bool Physics::init(PhysicsDebugDrawContext context)
	{
		m_physicsDebugDraw.init(std::move(context));

		Logger::logInfo("\'Physics\' successfully initialized!");

		b2Version version = b2GetVersion();
		Logger::logInfo(std::format("{} ~ Box2D version: {}.{}", 
			log_format::INDENT, 
			version.major, 
			version.minor));

		return true;
	}

	void Physics::shutdown()
	{
		//m_physics.shutdown(); Destroy all worlds? store them in physics?
	}
}