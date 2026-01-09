#pragma once
#include "engine/ecs/component/component_manager.hpp"
#include "engine/utils/type_traits.h"
#include "engine/utils/non_copyable.h"
#include <span>
#include <execution>

namespace cursed_engine
{
	// [Consider] - replacing SparseSet with ComponentMananger (hide impl)
	// [Consider] - have FindAllIf and FindIf - returning either pair<Entity, Ts&> or EntityHandle
	//  * caching component view (pass in sparse set, but pass in entities each frame?)
	//  * add sorting? and predicate (for ForEach) as optional, or separate function
	
	// private consturctor? only ECSRegistry cna construict?

	// TODO; make sure each type is unique!!
	template <ComponentType... Ts>
	class ComponentView //: private NonCopyable
	{
	public:
		// TODO, make private? friend class ECSRegistry..
		ComponentView(ComponentContainer<Ts>&... containers, std::span<const Entity> entities); 
		~ComponentView() = default;

		// Allow moving? Disable copying?
		ComponentView(ComponentView&&) = default;
		ComponentView& operator=(ComponentView&&) = default;
	 
		// ==================== Core API ====================
		template <typename Callback>  // TODO; replace typename with Callable (<Callable<Ts...>>), assure (force) reference? add predicate (ForEach if...)
		void forEach(Callback&& callback) const;

		template <typename Callback>
		void forEach(Callback&& callback); // TODO; add optional filtering!

		//template <ComponentType... Ts, CallableReturns<bool, EntityHandle> Func>
		//[[nodiscard]] std::optional<ComponentView<Ts...>> view(Func&& filtering) const;

		//template <ComponentType... Ts, CallableReturns<bool, EntityHandle> Func>
		//[[nodiscard]] ComponentView<Ts...> view(Func&& filtering);


		template <ComponentType T>
		[[nodiscard]] const T* getComponent(Entity entity) const;

		template <ComponentType T>
		[[nodiscard]] T* getComponent(Entity entity);

		// ==================== Queries ====================
		[[nodiscard]] bool contains(Entity entity) const noexcept;

		[[nodiscard]] bool isEmpty() const noexcept;

		[[nodiscard]] std::size_t size() const noexcept;

		[[nodiscard]] inline const std::span<Entity> getEntities() const { return m_entities; }

		// ==================== Search ====================
		template <typename Predicate> // template <Callable Func>
		[[nodiscard]] std::optional<Entity> findIf(Predicate&& predicate) const;

		template <typename Predicate>
		[[nodiscard]] std::vector<Entity> findAllIf(Predicate&& predicate) const;

		// ==================== Iterator ====================
		class Iterator
		{
		public:
			Iterator(ComponentView* view, std::size_t index = 0);

			Iterator& operator++();

			bool operator!=(const Iterator& other) const;

			const std::tuple<Ts*...> operator*() const;

			std::tuple<Ts*...> operator*();

		private:
			ComponentView* m_view;
			std::size_t m_index;
		};

		Iterator begin() { return Iterator(this); }
		Iterator end() { return Iterator(this, m_entities.size()); }

	private:
		// ==================== Helpers ====================
		std::tuple<const Ts&...> getComponents(EntityID id) const; // will this work with const TransformComponent and SpriteComponent

		std::tuple<Ts&...> getComponents(EntityID id);

		std::span<const Entity> m_entities; // TODO; or handles?
		std::tuple<ComponentContainer<Ts>&...> m_components;
	};

#pragma region Methods

	template <ComponentType ...Ts>
	ComponentView<Ts...>::ComponentView(ComponentContainer<Ts>&... containers, std::span<const Entity> entities)
	: m_components{ containers... }, m_entities{ entities }
	{
	}

	template <ComponentType ...Ts>
	template <typename Callback>
	void ComponentView<Ts...>::forEach(Callback&& callback) const // TODO; fix this function!
	{
		static_assert(!std::invocable<Callback, Ts&..., Entity> && "Entity can't be last argument in callback!");
		
		constexpr bool hasEntityParam = std::invocable<Callback, Entity, Ts&...>;

		for (const auto& entity : m_entities)
		{
			auto components = getComponents(entity.id);

			if constexpr (hasEntityParam)
			{
				// Check if Callback returns bool (breakable)
				if constexpr (std::is_invocable_r_v<bool, Callback, Entity, Ts&...>)
				{
					if (!std::apply([&](auto&&... components) { return std::invoke(callback, entity, components...); }, components))
					{
						break;
					}

				}
				else
				{
					std::apply([&](auto&&... components) { return std::invoke(callback, entity, components...); }, components);
				}
			}
			else
			{
				// Check if Callback returns bool (breakable)
				if constexpr (std::is_invocable_r_v<bool, Callback, Ts&...>)
				{
					if (!std::apply(callback, components))
					{
						break;
					}
				}
				else
				{
					std::apply(callback, components);
				}
			}
		}
	}

	template <ComponentType ...Ts>
	template <typename Callback>
	void ComponentView<Ts...>::forEach(Callback&& callback)
	{
		static_assert(!std::invocable<Callback, Ts&..., Entity> && "Entity can't be last argument in callback!");

		constexpr bool hasEntityParam = std::invocable<Callback, Entity, Ts&...>;

		// TODO; make functions safe for parallization... (Seq) seems to be fastest approach

		std::for_each(std::execution::seq, m_entities.begin(), m_entities.end(),
			[&](const Entity& entity) {
				auto components = getComponents(entity.id);

				if constexpr (hasEntityParam)
				{
					// Check if Callback returns bool (breakable)
					if constexpr (std::is_invocable_r_v<bool, Callback, Entity, Ts&...>)
					{
						if (!std::apply([&](auto&&... components) { return std::invoke(callback, entity, components...); }, components))
						{
							break;
						}
					}
					else
					{
						std::apply([&](auto&&... components) { return std::invoke(callback, entity, components...); }, components);
					}
				}
				else
				{
					// Check if Callback returns bool (breakable)
					if constexpr (std::is_invocable_r_v<bool, Callback, Ts&...>)
					{
						if (!std::apply(callback, components))
						{
							break;
						}
					}
					else
					{
						std::apply(callback, components);
					}
				}
			});
	}

	template <ComponentType ...Ts>
	template <ComponentType T>
	[[nodiscard]] const T* ComponentView<Ts...>::getComponent(Entity entity) const
	{
		if (contains(entity))
			return std::get<ComponentContainer<T>&>(m_components).get(entity.id);

		return nullptr;
	}

	template <ComponentType ...Ts>
	template <ComponentType T>
	[[nodiscard]] T* ComponentView<Ts...>::getComponent(Entity entity)
	{
		return const_cast<T*>(std::as_const(*this).template getComponent<T>(entity.id));
	}

	template <ComponentType ...Ts>
	[[nodiscard]] bool ComponentView<Ts...>::contains(Entity entity) const noexcept
	{
		return std::ranges::find(m_entities, entity) != m_entities.end();
	}

	template <ComponentType ...Ts>
	[[nodiscard]] bool ComponentView<Ts...>::isEmpty() const noexcept
	{
		return m_entities.empty();
	}

	template <ComponentType ...Ts>
	[[nodiscard]] std::size_t ComponentView<Ts...>::size() const noexcept
	{
		return m_entities.size();
	}

	template <ComponentType... Ts>
	template <typename Predicate> // template <Callable Func>
	std::optional<Entity> ComponentView<Ts...>::findIf(Predicate&& predicate) const
	{
		auto it = std::ranges::find_if(m_entities, [](Entity entity) 
			{
				return predicate(std::get<ComponentContainer<Ts>&>(m_components).at(entity.id)...); // TODO; pass in entity to?
			});

		if (it != m_entities.end())
			return *it;

		return std::nullopt;
	}

	template <ComponentType ...Ts>
	template <typename Predicate>
	std::vector<Entity> ComponentView<Ts...>::findAllIf(Predicate&& predicate) const
	{
		//static_assert(std::)

			// TODO; check that no components are unique... (needs to be same, or subset, of CompoenntView<Ts> 
			// TODO; allow (optional) entity argument?

		/*	std::vector<Entity> matchingEntities;

		std::copy_if(m_entities.begin(), m_entities.end(), std::back_inserter(matchingEntities),
			[&](const Entity& entity)
			{
				return predicate(std::get<ComponentContainer<Ts>&>(m_components).At(entity.ID)...);
			});

		return matchingEntities;*/

		return {}; // TODO; use span?
	}

	template <ComponentType ...Ts>
	std::tuple<const Ts&...> ComponentView<Ts...>::getComponents(EntityID id) const
	{
		return std::tie(std::get<ComponentContainer<Ts>&>(m_components).at(id)...);
	}

	template <ComponentType ...Ts>
	std::tuple<Ts&...> ComponentView<Ts...>::getComponents(EntityID id)
	{
		return std::tie(std::get<ComponentContainer<Ts>&>(m_components).at(id)...); // Use at instead?
	}


	// ==================== Iterator ====================

	template <ComponentType ...Ts>
	ComponentView<Ts...>::Iterator::Iterator(ComponentView* view, std::size_t index)
		: m_view{ view }, m_index{ index }
	{
	}
	
	template <ComponentType... Ts>
	typename ComponentView<Ts...>::Iterator& ComponentView<Ts...>::Iterator::operator++()
	{
		++m_index;
		return *this;
	}

	template <ComponentType... Ts>
	bool ComponentView<Ts...>::Iterator::operator!=(const Iterator& other) const
	{
		return m_index != other.m_index;
	}

	template <ComponentType... Ts>
	const std::tuple<Ts*...> ComponentView<Ts...>::Iterator::operator*() const
	{
		return m_view->GetComponents(m_view->m_entities[m_index]);
	}

	template <ComponentType... Ts>
	std::tuple<Ts*...> ComponentView<Ts...>::Iterator::operator*()
	{
		return m_view->GetComponents(m_view->m_entities[m_index]);
	}

#pragma endregion
}