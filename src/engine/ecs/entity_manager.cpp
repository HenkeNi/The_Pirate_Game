#include "C:/dev/repos/The_Pirate_Game/build/CMakeFiles/Engine.dir/Debug/cmake_pch.hxx"
#include "engine/ecs/entity/entity_manager.h"
#include "engine/core/logger.h"
#include <string>
//#include <ranges>

namespace cursed_engine
{
	EntityManager::EntityManager()
	{
		initializeAvailableIDs();
	}

	std::optional<Entity> EntityManager::create() noexcept
	{
		if (!m_available.empty())
		{
			const EntityID id = m_available.front();
			m_available.pop();

			m_alive.emplace(id, id, m_versions[id]);
			return Entity{ id, m_versions[id] };
		}

		Logger::logWarning("[EntityManager] Maximum entity capacity reached!");
		return std::nullopt;
	}

	bool EntityManager::destroy(Entity entity) noexcept
	{
		if (!isAlive(entity))
		{
			Logger::logError("[EntityManager] Invalid Entity " + std::to_string(entity.id) + "!");
			return false;
		}

		recycle(entity.id);
		m_alive.remove(entity.id);
		m_signatures.reset(entity.id);

		return true;
	}

	void EntityManager::destroyAll() noexcept
	{
		for (const auto& [id, version] : m_alive)
		{
			recycle(id);
		}

		m_signatures.resetAll();
		m_alive.clear();
	}

	bool EntityManager::isValidID(EntityID id) const noexcept
	{
		return id < MAX_ENTITIES && id >= 0;
	}

	bool EntityManager::isAlive(Entity entity) const noexcept
	{
		return isValidID(entity.id) &&
			m_alive.contains(entity.id) &&
			m_versions[entity.id] == entity.version;
	}

	EntitySignature EntityManager::getSignature(EntityID id) const noexcept
	{
		assert(isValidID(id) && "Trying to access signature with an invalid id!");
		return m_signatures.getSignature(id);
	}

	void EntityManager::setSignature(EntityID id, EntitySignature signature)
	{
		m_signatures.setSignature(id, signature);
	}

	std::span<const Entity> EntityManager::getEntities(EntitySignature signature) const noexcept
	{

		// sparse set

		return std::span<const Entity>();
	}

	void EntityManager::initializeAvailableIDs()
	{
		for (int i = 0; i < MAX_ENTITIES; ++i)
		{
			m_available.push(i);
			m_versions[i] = 0;
		}
	}

	void EntityManager::recycle(EntityID id)
	{
		assert(isValidID(id) && "Tried to recycle invalid id!");

		m_available.push(id);
		++m_versions[id];
	}
}
