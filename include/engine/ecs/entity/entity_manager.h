#pragma once
#include "../component/component_manager.h"
#include "../signature_registry.hpp"
#include "../ecs_types.h"
#include "engine/utils/non_copyable.h"
#include "entity.h"
#include <optional>
#include <queue>
#include <span>

namespace cursed_engine
{
	struct Entity;

	class EntityManager : private NonCopyable
	{
	public:
		// ==================== Construction/Destruction ====================

		EntityManager();
		~EntityManager() = default;

		EntityManager(EntityManager&&) = default;
		EntityManager& operator=(EntityManager&&) = default;

		// ==================== Lifecycle ====================

		[[nodiscard]] std::optional<Entity> create() noexcept; // safe to use noexcept? return entityHandle? return entity handle set to invalid, or nulloptr?

		bool destroy(Entity entity) noexcept;

		void destroyAll() noexcept;

		// ==================== Queries ====================

		[[nodiscard]] bool isValidID(EntityID id) const noexcept;

		[[nodiscard]] bool isAlive(Entity entity) const noexcept;

		// ==================== Statistics ====================

		[[nodiscard]] inline std::size_t getAliveSize() const noexcept { return m_alive.size(); }

		[[nodiscard]] inline std::size_t getAvailableSize() const noexcept { return m_available.size(); }

		// ==================== Signature Management ====================

		[[nodiscard]] EntitySignature getSignature(EntityID id) const noexcept;

		void setSignature(EntityID id, EntitySignature signature);

		// ==================== Entity Queries ====================

		[[nodiscard]] std::span<const Entity> getEntities(EntitySignature signature) const noexcept;

	private:
		// ==================== Internal Helpers ====================

		void initializeAvailableIDs();

		void recycle(EntityID id);

		// ==================== Data Members ====================

		sparse_set<Entity, EntityID> m_alive;
		std::queue<EntityID> m_available;

		SignatureRegistry<EntityID, MAX_COMPONENTS, MAX_ENTITIES> m_signatures;
		std::array<uint32_t, MAX_ENTITIES> m_versions;
	};
}