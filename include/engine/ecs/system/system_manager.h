#pragma once
#include "engine/utils/type_traits.h"
#include "engine/ecs/system/system.h"
#include "engine/utils/data_structures/sparse_set.hpp"
#include "engine/ecs/ecs_types.h"
#include <memory>

namespace cursed_engine
{
	class ECSRegistry;

	class SystemManager
	{
	public:

		template <DerivedFrom<System> T, typename... Args>
		T& emplace(Args&&... args);

		template <DerivedFrom<System> T>
		void insert(std::unique_ptr<T>&& system);

		template <DerivedFrom<System> T>
		const T& getSystem() const;

		template <DerivedFrom<System> T>
		T& getSystem();

		template <DerivedFrom<System> T>
		const T* tryGetSystem() const;

		template <DerivedFrom<System> T>
		T* tryGetSystem();

		template <DerivedFrom<System> T>
		bool contains() const noexcept;

		void update(ECSRegistry& registry, float deltaTime);

		void clear();

	private:
		//using Systems = std::vector<std::unique_ptr<System>>;
		using Systems = sparse_set<std::unique_ptr<System>, SystemID>;

		Systems m_systems;
	};

#pragma region Methods

	template <DerivedFrom<System> T, typename... Args>
	T& SystemManager::emplace(Args&&... args)
	{
		m_systems.emplace(getSystemID<T>(), std::forward<Args>(args)...);
	}

	template <DerivedFrom<System> T>
	void SystemManager::insert(std::unique_ptr<T>&& system)
	{
		m_systems.insert(getSystemID<T>(), std::move(system));
	}

	template <DerivedFrom<System> T>
	const T& SystemManager::getSystem() const
	{
		return m_systems.at(getSystemID<T>());
	}

	template <DerivedFrom<System> T>
	T& SystemManager::getSystem()
	{
		return m_systems.at(getSystemID<T>());
	}

	template <DerivedFrom<System> T>
	const T* SystemManager::tryGetSystem() const
	{
		return m_systems.get(getSystemID<T>());
	}

	template <DerivedFrom<System> T>
	T* SystemManager::tryGetSystem()
	{
		return m_systems.get(getSystemID<T>());
	}

	template <DerivedFrom<System> T>
	bool SystemManager::contains() const noexcept
	{
		return m_systems.contains(getSystemID<T>());
	}

#pragma endregion
}

// Opt 1; scene stack contains a systemManager

// op2; engine loop gets active scene's from scene stack... iterates over them... and push their entities into the system manager

// opt 3; static system manager in each scene...

// opt 4; pass in systemManager to each scene?