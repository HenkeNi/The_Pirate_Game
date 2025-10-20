#pragma once
#include "engine/ecs/ecs_types.h"
#include "engine/utils/type_traits.h"
#include "engine/utils/non_copyable.h"
#include "engine/utils/data_structures/sparse_set.hpp"

namespace cursed_engine
{
	template <ComponentType T>
	using ComponentContainer = SparseSet<std::decay_t<T>, EntityID>;

	// TODO; prefix with I for all interface classes?
	class IComponentManager
	{
	public:
		virtual ~IComponentManager() = default;
		
		// ==================== Component Management ====================
		virtual void remove(EntityID id) = 0;
		virtual void removeAll() = 0;

		// ==================== Queries ====================
		[[nodiscard]] virtual std::size_t size() const noexcept = 0;
		[[nodiscard]] virtual bool contains(EntityID id) const = 0;
	};

	template <ComponentType T>
	class ComponentManager : public IComponentManager, public NonCopyable
	{
	public:
		// ==================== Construction/Destruction ====================
		ComponentManager() = default;
		~ComponentManager() override = default;

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager(ComponentManager&&) = default;

		ComponentManager& operator=(const ComponentManager&) = delete;
		ComponentManager& operator=(ComponentManager&&) = default;

		// ==================== Component Operations ====================
		template <typename... Args>
		bool add(EntityID id, Args&&... args);

		void remove(EntityID id) override; // TODO, return bool? valid check?
		void removeAll() override;

		// ==================== Component Access ====================
		[[nodiscard]] const T& get(EntityID id) const;
		[[nodiscard]] T& get(EntityID id);

		[[nodiscard]] const T* tryGet(EntityID id) const;
		[[nodiscard]] T* tryGet(EntityID id);

		// ==================== Container Access ====================
		[[nodiscard]] const ComponentContainer<T>& getContainer() const;
		[[nodiscard]] ComponentContainer<T>& getContainer();

		// ==================== Capacity Management ====================
		void reserve(std::size_t capacity);

		// ==================== Query Operations ====================
		[[nodiscard]] std::size_t size() const noexcept override;
		[[nodiscard]] bool contains(EntityID id) const override;

	private:
		ComponentContainer<T> m_components;
	};

#pragma region Methods

	template <ComponentType T>
	template <typename... Args>
	bool ComponentManager<T>::add(EntityID id, Args&&... args)
	{
		return m_components.emplace(id, std::forward<Args>(args)...);
	}

	template <ComponentType T>
	void ComponentManager<T>::remove(EntityID id)
	{
		m_components.remove(id);
	}

	template <ComponentType T>
	void ComponentManager<T>::removeAll()
	{
		m_components.clear();
	}

	template <ComponentType T>
	const T& ComponentManager<T>::get(EntityID id) const
	{
		assert(m_components.contains(id) && "Entity does not have this component!");
		return m_components.at(id);
	}

	template <ComponentType T>
	T& ComponentManager<T>::get(EntityID id)
	{
		return const_cast<T&>(std::as_const(*this).get<T>(id));
	}

	template <ComponentType T>
	const T* ComponentManager<T>::tryGet(EntityID id) const
	{
		if (m_components.contains(id))
			return m_components.get(id);

		return nullptr;
	}

	template <ComponentType T>
	T* ComponentManager<T>::tryGet(EntityID id)
	{
		return const_cast<T*>(std::as_const(*this).tryGet(id));
	}

	template <ComponentType T>
	const ComponentContainer<T>& ComponentManager<T>::getContainer() const
	{
		return m_components;
	}

	template <ComponentType T>
	ComponentContainer<T>& ComponentManager<T>::getContainer()
	{
		return m_components;
	}

	template <ComponentType T>
	void ComponentManager<T>::reserve(std::size_t capacity)
	{
		m_components.reserve(capacity);
	}

	template <ComponentType T>
	std::size_t ComponentManager<T>::size() const noexcept
	{
		return m_components.size();
	}

	template <ComponentType T>
	bool ComponentManager<T>::contains(EntityID id) const
	{
		return m_components.contains(id);
	}

#pragma endregion
}