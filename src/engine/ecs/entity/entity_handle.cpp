#include "C:/dev/repos/The_Pirate_Game/build/CMakeFiles/Engine.dir/Debug/cmake_pch.hxx"
#include "engine/ecs/entity/entity_handle.h"


namespace cursed_engine
{
	EntityHandle::EntityHandle(Entity entity, ECSRegistry* registry)
		: m_entity{ entity }, m_registry{ registry }
	{
	}
}