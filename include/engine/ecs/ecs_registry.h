#pragma once
#include "entity/entity_manager.h"
#include "component/component_view.hpp"
#include "component/component_manager.hpp"
#include "engine/utils/non_copyable.h"
#include "engine/core/logger.h"
#include "ecs_types.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <cassert>

namespace cursed_engine
{
	/*
	* Option 2: Manually mark affected caches dirty during component operations
When adding/removing a component, just:
Iterate all active query caches
For each one, check if the entity might affect that query
Either: (newSig & querySig) == querySig
Or: compare old/new sig and see if the match status changed
Mark the cache dirty if so.
	*/

	// TODO; pass in entities into component view when creating one
	struct QueryCache {
		EntitySignature signature;
		std::vector<Entity> matchingEntities;

		bool dirty = true;
	};

	class EntityHandle;

	class ECSRegistry : public NonCopyable
	{
	public:

		// ==================== Entity Lifecycle ====================

		EntityHandle createEntity();

		bool destroyEntity(Entity entity);

		void destroyAllEntities();

		[[nodiscard]] inline bool isAlive(Entity entity) const noexcept { return m_entityManager.isAlive(entity); }

		// ==================== Entity Lookup / Metadata ====================

		std::optional<EntityHandle> FindEntityByName(std::string_view name);

		std::vector<EntityHandle> FindAllEntitiesWithTag(std::string_view tag);


		// ==================== Component Queries ====================

		template <ComponentType... Ts>
		[[nodiscard]] std::optional<ComponentView<Ts...>> view() const; // remove? since requires a const world!

		template <ComponentType... Ts>
		[[nodiscard]] ComponentView<Ts...> view();

		// ==================== Component Management ====================

		template <ComponentType T, typename... Args>
		std::pair<T*, bool> attachComponent(Entity entity, Args&&... args);

		template <ComponentType T>
		bool detachComponent(Entity entity);


		// ==================== Component Access ====================

		template <ComponentType T>
		[[nodiscard]] const T& getComponent(Entity entity) const;

		template <ComponentType T>
		[[nodiscard]] T& getComponent(Entity entity);

		template <ComponentType T>
		[[nodiscard]] const T* tryGetComponent(Entity entity) const;

		template <ComponentType T>
		[[nodiscard]] T* tryGetComponent(Entity entity);

		// ==================== Component Inspection ====================

		template <ComponentType... Ts>
		[[nodiscard]] bool hasComponents(Entity entity) const noexcept;

	private:
		// ==================== Internal Helpers ====================

		template <ComponentType T>
		[[nodiscard]] ComponentManager<T>& findOrCreateComponentManager();

		template <ComponentType T>
		[[nodiscard]] const ComponentManager<T>* getComponentManager() const;

		template <ComponentType... Ts>
		EntitySignature createEntitySignature() const;

		using ComponentManagers = std::vector<std::unique_ptr<class IComponentManager>>;
		using QueryCacheStorage = std::unordered_map<EntitySignature, QueryCache>;

		EntityManager m_entityManager;
		ComponentManagers m_componentManagers;
		QueryCacheStorage m_queryCacheStorage;
	};

#pragma region Methods

	template <ComponentType... Ts>
	[[nodiscard]] std::optional<ComponentView<Ts...>> ECSRegistry::view() const
	{
		const auto signature = createEntitySignature<Ts...>();
		const auto entities = m_entityManager.getEntities(signature);

		bool allManagersExist = ((getComponentManager<Ts>() != nullptr) && ...);

		if (!allManagersExist)
		{
			Logger::logError("[ECSRegistry::view] - One or more required component managers do not exist!");
			return std::nullopt;
		}

		ComponentView<Ts...> componentView{ getComponentManager<Ts>()->getContainer()..., std::move(entities) };
		return componentView;
	}

	template <ComponentType... Ts>
	[[nodiscard]] ComponentView<Ts...> ECSRegistry::view()
	{
		return const_cast<const ECSRegistry*>(this)->view<Ts...>().value(); // works?

	}

	template <ComponentType T, typename... Args>
	std::pair<T*, bool> ECSRegistry::attachComponent(Entity entity, Args&&... args)
	{
		if (!m_entityManager.isAlive(entity))
		{
			Logger::logWarning("[ECSRegistry::attachComponent] - using an invalid or dead entity!" + std::to_string(entity.id));
			return { nullptr, false };
		}

		if (hasComponents<T>())
		{
			Logger::logWarning("[ECSRegistry::attachComponent] - entity already have component!");
			return { nullptr, false };
		}

		auto& componentManager = findOrCreateComponentManager<T>();

		auto [component, success] = componentManager.addComponent(entity.id, std::forward<Args>(args)...);

		if (!success)
		{
			Logger::logWarning("[ECSRegistry::attachComponent] - failed to attach component!");
			return { nullptr, false };
		}

		auto signature = m_entityManager.getSignature(entity.id);
		signature.set(getComponentID<T>());

		m_entityManager.setSignature(entity, signature);
		return { component, true };
	}

	template <ComponentType T>
	bool ECSRegistry::detachComponent(Entity entity)
	{
		if (!m_entityManager.isAlive(entity))
		{
			Logger::logWarning("[ECSRegistry::detachComponent] - using an invalid or dead entity!" + std::to_string(entity.id));
			return false;
		}

		auto* componentManager = getComponentManager<T>();

		if (!componentManager) [[unlikely]]
		{
			Logger::logError("[ECSRegistry::detachComponent] - failed to fetch component manager!");
			return false;
		} 

		componentManager->remove(entity.id);

		auto signature = m_entityManager.getSignature(entity.id);
		signature.set(getComponentID<T>(), false);

		m_entityManager.setSignature(entity, signature);

		return false;
	}

	template <ComponentType T>
	const T& ECSRegistry::getComponent(Entity entity) const
	{
		assert(m_entityManager.isAlive(entity)) && "Tried to call 'getComponent' with an invalid entity!";
		assert(hasAllComponents<T>(entity.id) && "Entity doesn't have the required component!");

		return getComponentManager<T>()->GetComponent(entity.id);
	}

	template <ComponentType T>
	[[nodiscard]] T& ECSRegistry::getComponent(Entity entity)
	{
		return const_cast<T&>(std::as_const(*this).getComponent<T>(entity));
	}

	template <ComponentType T>
	[[nodiscard]] const T* ECSRegistry::tryGetComponent(Entity entity) const
	{
		if (!m_entityManager.isAlive(entity))
		{
			Logger::LogWarning("[ECS::TryGetComponent] - Tried access component with an invalid entity!");
			return nullptr;
		}

		auto* componentManager = getComponentManager<T>();

		if (!componentManager) [[unlikely]]
		{
			Logger::LogWarning("[ECS::TryGetComponent] - ComponentManager for type not found!");
			return nullptr;
		}

		return componentManager->tryGet(entity.id);
	}

	template <ComponentType T>
	[[nodiscard]] T* ECSRegistry::tryGetComponent(Entity entity)
	{
		return const_cast<T*>(std::as_const(*this).tryGetComponent<T>(entity));
	}

	template <ComponentType... Ts>
	[[nodiscard]] bool ECSRegistry::hasComponents(Entity entity) const noexcept
	{
		if (!m_entityManager.isAlive(entity))
		{
			Logger::logWarning("[ECSRegistry::hasComponents] - using an invalid or dead entity!");
			return false;
		}

		EntitySignature currentSignature = m_entityManager.getSignature(entity);
		EntitySignature requiredSignature = buildSignature<Ts>();

		return (currentSignature & requiredSignature) == requiredSignature;
	}

	template <ComponentType T>
	[[nodiscard]] ComponentManager<T>& ECSRegistry::findOrCreateComponentManager()
	{
		ComponentID id = getComponentID<T>();

		if (id >= m_componentManagers.size()) 
		{
			m_componentManagers.resize(id + 1);
		}

		if (!m_componentManagers[id])
		{
			m_componentManagers[id] = std::make_unique<ComponentManager<T>>();
		}

		return static_cast<ComponentManager<T>&>(*m_componentManagers[id]);
	}

	template <ComponentType T>
	[[nodiscard]] const ComponentManager<T>* ECSRegistry::getComponentManager() const
	{
		ComponentID id = getComponentID<T>();

		if (id >= m_componentManagers.size() || !m_componentManagers[id])
		{
			Logger::logWarning("[ECSRegistry::getComponentManager] - Tried to access invalid component manager!");
			return nullptr;
		}

		return static_cast<const ComponentManager<T>*>(m_componentManagers.at(id).get());
	}

	template <ComponentType ...Ts>
	EntitySignature ECSRegistry::createEntitySignature() const
	{
		EntitySignature signature;
		(signature.set(getComponent<Ts>()), ...);

		return signature;
	}

#pragma endregion
}