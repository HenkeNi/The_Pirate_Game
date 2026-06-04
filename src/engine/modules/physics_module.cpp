#include "engine/modules/physics_module.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	PhysicsModule::PhysicsModule()
		: m_physics{}
	{
	}

	bool PhysicsModule::init()
	{
		if (!m_physics.init())
			return false;

		Logger::logInfo("-> PhysicsModule: Success");
		return true;
	}

	void PhysicsModule::shutdown()
	{

	}

	//PhysicsServices PhysicsModule::getServices() noexcept
	//{
	//	return { &m_physics };
	//}
}