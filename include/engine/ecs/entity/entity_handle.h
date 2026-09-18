#pragma once
#include "engine/ecs/entity/entity.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/utils/concepts.h"

namespace cursed_engine
{
	// TODO; add hash?
	// Rename Entity? stores entity id and generation... and pointer to registry?
	// delete copy?
	class EntityHandle
	{
	public:
		EntityHandle();
		EntityHandle(Entity entity, ECSRegistry* registry);
		~EntityHandle() = default;

		EntityHandle(const EntityHandle&) = default;
		EntityHandle(EntityHandle&&) = default;

		EntityHandle& operator=(const EntityHandle&) = default;
		EntityHandle& operator=(EntityHandle&&) = default;

		[[nodiscard]] inline static EntityHandle invalid() noexcept { return EntityHandle{ Entity::invalid(), nullptr }; };

		template <ComponentType T, typename... Args>
		std::pair<T*, bool> attachComponent(Args&&... args); // rename addComponent?

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


		// add destroy... (cant store as const then)? - strore bool 'alive'?

		[[nodiscard]] inline const Entity getEntity() const noexcept { return m_entity; } // remove

		[[nodiscard]] bool isValid() const noexcept;

	private:
		ECSRegistry* m_registry;
		Entity m_entity;
	};

	/*struct EntityHash
	{
		size_t operator()(const Entity& e) const noexcept
		{
			return (size_t(e.id) << 32) | e.generation;
		}
	};*/

#pragma region Definitions

	template <ComponentType T, typename... Args>
	std::pair<T*, bool> EntityHandle::attachComponent(Args&&... args)
	{
		assert(isValid() && "Invalid Entity!");
		
		if (isValid())
			return m_registry->attachComponent<T>(m_entity, std::forward<Args>(args)...);

		// log error!
		return { nullptr, false };
	}

	template <ComponentType T>
	void EntityHandle::detachComponent()
	{
		assert(m_registry && "Invalid ECSRegistry!");
		m_registry->detachComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] const T& EntityHandle::getComponent() const
	{
		assert(m_registry && "Invalid ECSRegistry!");
		return m_registry->getComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] T& EntityHandle::getComponent()
	{
		assert(m_registry && "Invalid ECSRegistry!");
		return m_registry->getComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] const T* EntityHandle::tryGetComponent() const
	{
		assert(m_registry && "Invalid ECSRegistry!");
		return m_registry->tryGetComponent<T>(m_entity);
	}

	template <ComponentType T>
	[[nodiscard]] T* EntityHandle::tryGetComponent()
	{
		assert(m_registry && "Invalid ECSRegistry!");
		return m_registry->tryGetComponent<T>(m_entity);
	}

	template <ComponentType ...Ts>
	bool EntityHandle::hasComponents() const
	{
		assert(m_registry && "Invalid ECSRegistry!");
		return m_registry->hasComponents<Ts...>(m_entity);
	}

#pragma endregion
}