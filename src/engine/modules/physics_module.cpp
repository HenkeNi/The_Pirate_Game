#include "engine/modules/physics_module.h"
#include "engine/core/logger.h"
#include <format>

namespace cursed_engine
{
	PhysicsModule::PhysicsModule()
		: m_physics{}
	{
	}

	bool PhysicsModule::init()
	{
		Logger::logInfo(std::format("{}[PhysicsModule] - Initialization started...", log_format::INDENT));
		//if (!m_physics.init())
		//	return false;

		Logger::logInfo(std::format("{}[PhysicsModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void PhysicsModule::shutdown()
	{
		//m_physics.shutdown(); Destroy all worlds? store them in physics?
	}

	//PhysicsServices PhysicsModule::getServices() noexcept
	//{
	//	return { &m_physics };
	//}
}