#pragma once
#include "engine/physics/physics.h"

namespace cursed_engine
{
	class PhysicsModule
	{
	public:
		PhysicsModule();

		bool init();
		void shutdown();

		[[nodiscard]] inline Physics& getPhysics() noexcept { return m_physics; }

	private:
		Physics m_physics;
	};
}