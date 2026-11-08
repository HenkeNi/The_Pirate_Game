//#include "C:/dev/repos/The_Pirate_Game/build/CMakeFiles/Engine.dir/Debug/cmake_pch.hxx"
#include "engine/ecs/entity/entity_handle.h"


namespace cursed_engine
{
	EntityHandle::EntityHandle()
		: m_entity{}, m_registry{ nullptr }
	{
	}

	EntityHandle::EntityHandle(Entity entity, ECSRegistry* registry)
		: m_entity{ entity }, m_registry{ registry }
	{
	}

	bool EntityHandle::isValid() const noexcept
	{
		return m_registry && m_registry->isAlive(m_entity);
	}


	bool EntityHandle::operator==(const EntityHandle& other) const noexcept
	{
		return m_entity == other.m_entity;
	}

	bool EntityHandle::operator==(const Entity& other) const noexcept
	{
		return m_entity == other;
	}

	bool EntityHandle::operator!=(const EntityHandle& other) const noexcept
	{
		return !(*this == other);
	}

	bool EntityHandle::operator!=(const Entity& other) const noexcept
	{
		return !(*this == other);
	}
}