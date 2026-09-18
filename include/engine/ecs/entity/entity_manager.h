#pragma once
#include "engine/ecs/signature_registry.hpp"
#include "engine/utils/containers/sparse_set.hpp"
#include "engine/ecs/ecs_types.h"
#include "entity.h"
#include <optional>
#include <queue>
#include <span>

namespace cursed_engine
{
	struct Entity;

	class EntityManager
	{
	public:
		// ==================== Construction/Destruction ====================
		EntityManager();
		~EntityManager() = default;

		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = default;

		EntityManager& operator=(const EntityManager&) = delete;
		EntityManager& operator=(EntityManager&&) = default; // or delete move?

		// ==================== Lifecycle ====================

		[[nodiscard]] std::optional<Entity> create() noexcept; // safe to use noexcept? return entityHandle? return entity handle set to invalid, or nulloptr?

		bool destroy(Entity entity) noexcept;

		void destroyAll() noexcept;

		// ==================== Queries ====================

		[[nodiscard]] bool isValidId(EntityId id) const noexcept;

		[[nodiscard]] bool isAlive(Entity entity) const noexcept;

		// ==================== Statistics ====================

		[[nodiscard]] inline std::size_t getAliveSize() const noexcept { return m_alive.size(); }

		[[nodiscard]] inline std::size_t getAvailableSize() const noexcept { return m_available.size(); }

		// ==================== Signature Management ====================
	
		[[nodiscard]] EntitySignature getSignature(EntityId id) const noexcept; 

		[[nodiscard]] inline const SignatureRegistry<EntityId, MAX_COMPONENTS, MAX_ENTITIES>* getSignatureRegistry() const { return &m_signatures; }

		[[nodiscard]] bool hasSignature(EntityId id, EntitySignature signature) const noexcept;

		void setSignature(EntityId id, EntitySignature signature);

		// ==================== Entity Queries ====================

		[[nodiscard]] const std::vector<Entity> getEntities(EntitySignature signature) const noexcept;

	private:
		// ==================== Internal Helpers ====================

		void initializeAvailableIds();

		void recycle(EntityId id);

		// ==================== Data Members ====================

		sparse_set<Entity, EntityId> m_alive;
		std::queue<EntityId> m_available;

		SignatureRegistry<EntityId, MAX_COMPONENTS, MAX_ENTITIES> m_signatures;
		std::array<uint32_t, MAX_ENTITIES> m_versions;
	};
}