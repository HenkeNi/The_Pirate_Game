#include "engine/modules/physics_module.h"
#include "engine/core/logger.h"
#include <format>

namespace cursed_engine
{
	bool PhysicsModule::init(PhysicsDebugDrawContext context)
	{
		Logger::logInfo(std::format("{}[PhysicsModule] - Initialization started...", log_format::INDENT));
		//if (!m_physics.init())
		//	return false;

		// auto version = b2GetVersion(); -> log this in physics?


		m_physicsDebugDraw.init(std::move(context));

		Logger::logInfo(std::format("{}[PhysicsModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void PhysicsModule::shutdown()
	{
		//m_physics.shutdown(); Destroy all worlds? store them in physics?
	}

	//void PhysicsModule::debugDraw()
	//{
	//	m_physicsDebugDraw.draw();
	//}

	//PhysicsServices PhysicsModule::getServices() noexcept
	//{
	//	return { &m_physics };
	//}
}