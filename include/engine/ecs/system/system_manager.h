#pragma once
#include "engine/ecs/system/system.h"
#include "engine/ecs/ecs_types.h"
#include "engine/utils/containers/sparse_set.hpp"
#include "engine/utils/concepts.h"
#include <memory>

namespace cursed_engine
{
	class ECSRegistry;

	class SystemManager
	{
	public:
		void update(SystemUpdateContext& context); // make private? friend class Egnein?
		void render(SystemRenderContext& context);

		void clear();

		template <DerivedFrom<UpdateSystem> T, typename... Args>
		T& emplace(Args&&... args);

		template <DerivedFrom<RenderSystem> T, typename... Args>
		T& emplace(Args&&... args);

		template <DerivedFrom<UpdateSystem> T>
		void insert(std::unique_ptr<T>&& system);

		template <DerivedFrom<RenderSystem> T>
		void insert(std::unique_ptr<T>&& system);

		template <DerivedFrom<UpdateSystem> T>
		[[nodiscard]] const T& getSystem() const;

		template <DerivedFrom<RenderSystem> T>
		[[nodiscard]] const T& getSystem() const;

		template <DerivedFrom<UpdateSystem> T>
		[[nodiscard]] T& getSystem();

		template <DerivedFrom<RenderSystem> T>
		[[nodiscard]] T& getSystem();

		template <DerivedFrom<UpdateSystem> T>
		[[nodiscard]] const T* tryGetSystem() const;

		template <DerivedFrom<RenderSystem> T>
		[[nodiscard]] const T* tryGetSystem() const;

		template <DerivedFrom<UpdateSystem> T>
		[[nodiscard]] T* tryGetSystem();

		template <DerivedFrom<RenderSystem> T>
		[[nodiscard]] T* tryGetSystem();

		template <DerivedFrom<UpdateSystem> T>
		[[nodiscard]] bool contains() const noexcept;

		template <DerivedFrom<RenderSystem> T>
		[[nodiscard]] bool contains() const noexcept;

	private:
		using UpdateSystems = sparse_set<std::unique_ptr<UpdateSystem>, SystemId>;
		using RenderSystems = sparse_set<std::unique_ptr<RenderSystem>, SystemId>;

		UpdateSystems m_updateSystems;
		RenderSystems m_renderSystems;
	};

#pragma region Definitions

	template <DerivedFrom<UpdateSystem> T, typename... Args>
	T& SystemManager::emplace(Args&&... args)
	{
		SystemId systemId = getUpdateSystemId<T>();

		assert(!m_updateSystems.contains(systemId) && "System already exist!");

		std::unique_ptr<T> system = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = system.get();

		m_updateSystems.insert(systemId, std::move(system));

		return *ptr;
	}

	template <DerivedFrom<RenderSystem> T, typename... Args>
	T& SystemManager::emplace(Args&&... args)
	{
		SystemId systemId = getRenderSystemId<T>();

		assert(!m_renderSystems.contains(systemId) && "System already exist!");

		std::unique_ptr<T> system = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = system.get();

		m_renderSystems.insert(getRenderSystemId<T>(), std::move(system));

		return *ptr;
	}

	template <DerivedFrom<UpdateSystem> T>
	void SystemManager::insert(std::unique_ptr<T>&& system)
	{
		SystemId systemId = getUpdateSystemId<T>();
		assert(!m_updateSystems.contains(systemId) && "System already exist!");

		m_updateSystems.insert(systemId, std::move(system));
	}

	template <DerivedFrom<RenderSystem> T>
	void SystemManager::insert(std::unique_ptr<T>&& system)
	{
		SystemId systemId = getRenderSystemId<T>();
		assert(!m_renderSystems.contains(systemId) && "System already exist!");

		m_renderSystems.insert(systemId, std::move(system));
	}

	template <DerivedFrom<UpdateSystem> T>
	const T& SystemManager::getSystem() const
	{
		return static_cast<const T&>(*m_updateSystems.at(getUpdateSystemId<T>())); // correct cast? or return static_cast<const T&>???????????????
	}

	template <DerivedFrom<RenderSystem> T>
	const T& SystemManager::getSystem() const
	{
		return static_cast<const T&>(*m_renderSystems.at(getRenderSystemId<T>())); // correct cast?
	}

	template <DerivedFrom<UpdateSystem> T>
	T& SystemManager::getSystem()
	{
		return const_cast<T&>(std::as_const(*this).getSystem<T>());
	}

	template <DerivedFrom<RenderSystem> T>
	T& SystemManager::getSystem()
	{
		return const_cast<T&>(std::as_const(*this).getSystem<T>());
	}

	template <DerivedFrom<UpdateSystem> T>
	const T* SystemManager::tryGetSystem() const
	{
		return static_cast<const T*>(m_updateSystems.at(getUpdateSystemId<T>()).get()); // correct cast?
	}

	template <DerivedFrom<RenderSystem> T>
	const T* SystemManager::tryGetSystem() const
	{
		return static_cast<const T*>(m_renderSystems.at(getRenderSystemId<T>()).get()); // correct cast?
	}

	template <DerivedFrom<UpdateSystem> T>
	T* SystemManager::tryGetSystem()
	{
		return const_cast<T*>(std::as_const(*this).tryGetSystem<T>());
	}

	template <DerivedFrom<RenderSystem> T>
	T* SystemManager::tryGetSystem()
	{
		return const_cast<T*>(std::as_const(*this).tryGetSystem<T>());
	}

	template <DerivedFrom<UpdateSystem> T>
	bool SystemManager::contains() const noexcept
	{
		SystemId id = getUpdateSystemId<T>();
		return m_updateSystems.contains(id);
	}

	template <DerivedFrom<RenderSystem> T>
	bool SystemManager::contains() const noexcept
	{
		SystemId id = getRenderSystemId<T>();
		return m_renderSystems.contains(id);
	}

#pragma endregion
}