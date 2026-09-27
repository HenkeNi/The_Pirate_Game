#include "game/systems/hud_system.h"
#include "game/events/events.h"
#include "game/components/components.h"
#include <engine/core/logger.h>
#include <engine/core/events/event_bus.h>
#include <engine/ecs/entity/entity_factory.h>
#include <engine/ecs/component/core_components.h>
#include <cassert>

HUDSystem::HUDSystem(ce::EventBus& eventBus, ce::EntityFactory& factory)
	: m_eventBus{ eventBus }, m_entityFactory{ factory }, m_ecsRegistry{ nullptr }
{
	m_eventBus.subscribe<SceneTransitionEvent>([&](const SceneTransitionEvent& e) 
		{
			if (!m_ecsRegistry)
			{
				return;
			}

			if (e.scene == "overworld_scene")
			{
				auto entities = m_ecsRegistry->view<PlayerComponent>();

				ce::Entity player = ce::Entity::invalid();

				entities.forEach([&](ce::Entity entity, const PlayerComponent& playerComponent)
					{
						// TODO; find correct player!

						player = entity;
						return;
					});

				if (!player.isValid())
				{
					return;
				}

				if (const HealthComponent* healthComponent = m_ecsRegistry->tryGetComponent<HealthComponent>(player))
				{
					createHealthContainer(healthComponent->currentLife, /*healthComponent->maxLife*/10);
				}

				// or read from a shared game state? check if player actualy have the component!
				//const HealthComponent& healthComponent = m_ecsRegistry->getComponent<HealthComponent>(player);
				

				// auto first = entity.begin();
				

				// Construct HUD for overworld scene
				//m_entityFactory.createFromPrefab(); // Heart container as a prefab?
				
			}

		});
}

void HUDSystem::update(cursed_engine::SystemUpdateContext& context)
{
	// TODO; probably need to be able to update max health and current health at runtime!
	// OR; construct HUD at start... hide some elements (unlocked heart containers) => need to "unlock" at runtime (undimmm damaged)


	// at game start... constructs the ui... reads max health and current health from player. -> how to "find" player?
		// tag component?
}

void HUDSystem::setECSRegistry(ce::ECSRegistry* registry)
{
	m_ecsRegistry = registry;
}

void HUDSystem::createHealthContainer(int currentHealth, int maxHealth)
{
	ce::EntityHandle heartContainerHandle = m_entityFactory.create(); // maybe this is specified in Scene's json?

	assert(heartContainerHandle.isValid());

	if (!heartContainerHandle.isValid())
	{
		ce::Logger::logError("Failed to create heart container");
		return;
	}

	auto [layoutComponent, success] = heartContainerHandle.attachComponent<ce::LayoutComponent>();

	if (!success)
	{
		return;
	}

	layoutComponent->direction = ce::LayoutComponent::Direction::Horizontal;

	for (int i = 0; i < maxHealth; ++i)
	{
		std::optional<ce::EntityHandle> heartHandle = m_entityFactory.createFromPrefab("heart", ce::FVec2{ 10.f, 10.f });
		
		if (heartHandle)
		{
			if (ce::TransformComponent* transformComponent = heartHandle.value().tryGetComponent<ce::TransformComponent>())
			{
				const float heartOffset = 25.f;
				transformComponent->position.x = float(i) * heartOffset; // TODO; use size plus an offset...
			}

			// give prefab either a heirarchy component, or a parent component...

			// name result or success?
			auto [hierarchyComponent, result] = heartHandle.value().attachComponent<ce::HierarchyComponent>();
			if (result)
			{
				hierarchyComponent->parent = heartContainerHandle;
			}
		}
	}

	// for each health in player... (how to get player? find with tag component? - player component)
}