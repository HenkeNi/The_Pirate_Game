#include "game/systems/map_system.h"
#include "game/map/map_generator.h"
#include "game/map/tile_map.h"
#include "game/events/events.h"
#include <engine/ecs/component/core_components.h>
#include <engine/core/events/event_bus.h>
#include <engine/core/logger.h>
#include <format>

MapSystem::MapSystem(MapGenerator& mapGenerator, cursed_engine::EventBus* eventBus)
	: m_mapGenerator{ mapGenerator }, m_tileMap{ nullptr }, m_eventBus{ eventBus }
{ 
}

void MapSystem::update(cursed_engine::SystemContext& context)
{
	// player or camera? 
	auto view = context.registry.view<cursed_engine::CameraComponent>();
	
	auto activeCamera = view.findFirst([](const cursed_engine::CameraComponent& cameraComponent)
		{
			return cameraComponent.isActive;
		});

	cursed_engine::FVec2 worldPosition{}; // create FVec2::zero();??

	if (activeCamera.has_value())
	{
		const auto& cameraTransformComponent = context.registry.getComponent<cursed_engine::TransformComponent>(activeCamera.value());
		worldPosition = cameraTransformComponent.position;
	}

	const auto& [xCoord, yCoord] = getMapChunkCoordinatesFromWorldPosition(worldPosition);
	//cursed_engine::Logger::logInfo(std::format("Coords: {}, {}", xCoord, yCoord));
	
	if (!m_tileMap->isValidChunk(xCoord, yCoord))
	{
		// generate new mapchunk
		auto mapChunk = m_mapGenerator.generateMapChunk(xCoord, yCoord);
		m_tileMap->insertMapChunk(std::move(mapChunk));

		m_eventBus->publishInstantly<MapChunkCreatedEvent>(xCoord, yCoord);
	}

	//auto* chunk = m_tileMap->getChunkAtPosition((int)worldPosition.x, (int)worldPosition.y); // pass floats?
	//
	//if (chunk)
	//{
	//	int x = 20;
	//}
	//else
	//{
	//	int x = 20;
	//	// get coordinates... from position 
	//	 
	//	
	//	// 
	//	// m_mapGenerator.generateMapChunk(x, y);
	//	// m_tileMap->insertMapChunk(std::move(mapChunk));
	//}
	// check if needing to generate new chunk...
	// 
	// m_mapGenerator.

}

void MapSystem::setMap(TileMap* map)
{
	m_tileMap = map;
}