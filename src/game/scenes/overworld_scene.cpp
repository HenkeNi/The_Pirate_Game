#include "game/scenes/overworld_scene.h"
#include "game/systems/map_render_system.h"
#include <engine/core/logger.h>
#include <engine/ecs/system/system_manager.h>

#include "game/systems/hud_system.h"
#include "game/systems/map_decoration_system.h"

#include "game/systems/map_system.h"
#include "game/events/events.h"


#include <engine/ecs/entity/entity_factory.h>
#include <engine/core/events/event_bus.h>
#include <engine/assets/asset_manager.h>

#include <engine/physics/physics.h>
#include <engine/ecs/system/physics_system.h>
#include "game/components/components.h" // remove later? 
#include <engine/ecs/component/core_components.h>

#include <engine/audio/audio_controller.h>

using namespace cursed_engine;

OverworldScene::OverworldScene(SceneContext context, ce::PhysicsAPI physicsAPI)
	: Scene{ std::move(context) }, m_physicsAPI{ physicsAPI }, m_physicsWorld{ FVec2{ 0, 0 } }
{
}

void OverworldScene::onUpdate(float deltaTime)
{

	int x = 20;
	//m_physicsWorld.step(); // here or physics system?
}

void OverworldScene::onEnter()
{
	Logger::logInfo("Entering Overworld Scene..."); // put in scene stack?

	auto audioHandle = m_context.audioManager->getHandle(cursed_engine::AudioDescriptor{ "../assets/sounds/theme/516076__breviceps__pirate-band-performs-drunken-sailor.wav" });

	if (audioHandle.isValid())
	{
		auto& audio = m_context.audioManager->get(audioHandle);
		m_context.audioController.play(audio, cursed_engine::AudioType::Music);
	}

	m_physicsAPI.setWorldId(m_physicsWorld.getWorldId()); // MAYBE PASS WORLD INSTEAD? AND PHYSICS API extracts it??

	m_context.entityFactory->setEcsRegistry(&m_registry);
	m_context.entityFactory->setPostInitContext({ &m_physicsWorld });
	m_context.systemManager->getSystem<HUDSystem>().setECSRegistry(&m_registry);

	m_context.systemManager->getSystem<PhysicsSystem>().setPhysicsWorld(&m_physicsWorld);

	m_mapGenerator.generateStartArea(m_tilemap, 1); // Don't do here? handle in new game event instead

	auto tilesetHandle = m_context.assetManager->getAssetHandle<Tileset>("overworld");

	assert(tilesetHandle.isValid() && "[Overworld Scene] - Not valid tileset!");

	if (!tilesetHandle.isValid())
	{
		Logger::logError("[OverworldScene::onEnter] - Invalid tileset handle");
		int x = 20;
	}

	const Tileset& tileset = m_context.assetManager->getAsset<Tileset>(tilesetHandle);

	auto& mapSystem = m_context.systemManager->emplace<MapSystem>(m_mapGenerator, m_context.eventBus);
	mapSystem.setTilemap(&m_tilemap); // NOTE! need to set map every time changing scene!

	auto& mapDecorationSystem = m_context.systemManager->getSystem<MapDecorationSystem>();
	mapDecorationSystem.setTilemap(&m_tilemap);
	mapDecorationSystem.setTileset(&tileset);

	auto& mapRenderSystem = m_context.systemManager->getSystem<MapRenderSystem>();
	mapRenderSystem.setTilemap(&m_tilemap);
	mapRenderSystem.setTileset(&tileset);

	m_context.eventBus->publishInstantly<MapChunkCreatedEvent>(0, 0);

	


	// m_context.entityFactory->createFromPrefab("raft", ce::FVec2{ 10.f, 20.f });

	

	// TODO; Maybe attach player (as Target) in Scene?
	auto playerEntities = m_registry.view<PlayerComponent>();

	ce::EntityHandle playerHandle = ce::EntityHandle::invalid();

	playerEntities.forEach([&](ce::Entity entity, const PlayerComponent& playerComponent)
		{
			// TODO; find correct player!

			playerHandle = ce::EntityHandle{ entity, &m_registry };
			return;
		});

	if (!playerHandle.isValid())
	{
		return;
	}


	auto cameraEntities = m_registry.view<ce::CameraComponent>();

	ce::EntityHandle cameraHandle = ce::EntityHandle::invalid();

	cameraEntities.forEach([&](ce::Entity entity, const ce::CameraComponent& cameraComponent)
		{
			// TODO; find correct player!

			cameraHandle = ce::EntityHandle{ entity, &m_registry };
			return;
		});

	if (!cameraHandle.isValid())
	{
		return;
	}

	cameraHandle.getComponent<ce::FollowComponent>().target = playerHandle;

	m_context.entityFactory->createFromPrefab("spider", FVec2{ 0.f, 0.f });
}

void OverworldScene::onExit()
{
}