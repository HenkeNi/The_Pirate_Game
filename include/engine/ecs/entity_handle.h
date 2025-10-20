#pragma once
#include "entity.h"
#include "ecs_registry.h"
#include "engine/utils/non_copyable.h"
#include "engine/utils/type_traits.h"

namespace cursed_engine
{
	// TODO; add hash?

	class EntityHandle : public NonCopyable
	{
	public:
		EntityHandle(Entity entity, ECSRegistry* registry);
		~EntityHandle() = default;

		EntityHandle(const EntityHandle&) = default;
		EntityHandle(EntityHandle&&) = default;

		EntityHandle& operator=(const EntityHandle&) = default;
		EntityHandle& operator=(EntityHandle&&) = default;

		template <ComponentType T, typename... Args>
		void attachComponent(Args&&... args);

		template <ComponentType T>
		void detachComponent();

		template <ComponentType T>
		[[nodiscard]] const T& getComponent() const;

		template <ComponentType T>
		[[nodiscard]] T& getComponent();

		template <ComponentType T>
		[[nodiscard]] const T* tryGetComponent() const;

		template <ComponentType T>
		[[nodiscard]] T* tryGetComponent();

		template <ComponentType... Ts>
		[[nodiscard]] bool hasComponents() const;

		[[nodiscard]] inline Entity GetEntity() const noexcept { return m_entity; }

	private:
		ECSRegistry* m_registry;
		Entity m_entity;
	};

#pragma region Methods

	template <ComponentType T, typename... Args>
	void EntityHandle::attachComponent(Args&&... args)
	{
		assert(m_registry && "Invalid ECSRegistry!");
		m_registry->attachComponent<T>(m_entity, std::forward<Args>()...);
	}

	template <ComponentType T>
	void EntityHandle::detachComponent()
	{
		m_registry->detachComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] const T& EntityHandle::getComponent() const
	{
		return m_registry->getComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] T& EntityHandle::getComponent()
	{
		return m_registry->getComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] const T* EntityHandle::tryGetComponent() const
	{
		return m_registry->tryGetComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] T* EntityHandle::tryGetComponent()
	{
		return m_registry->tryGetComponent<T>(m_entity);
	}

	template <ComponentType ...Ts>
	bool EntityHandle::hasComponents() const
	{
		return m_registry->hasComponents<Ts...>(m_entity.id);
	}

#pragma endregion
}