#include "engine/core/subsystem_registry.h"

namespace cursed_engine
{
	bool SubsystemRegistry::isEmpty() const noexcept
	{
		return m_subsystems.empty();
	}

	std::size_t SubsystemRegistry::size() const noexcept
	{
		return m_subsystems.size();
	}
}