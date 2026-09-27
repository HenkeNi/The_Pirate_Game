#include "game/systems/map_system.h"
#include "game/map/map_generator.h"
#include "game/map/tilemap.h"
#include "game/events/events.h"
#include <engine/ecs/component/core_components.h>
#include <engine/core/events/event_bus.h>
#include <engine/core/logger.h>
#include <format>

MapSystem::MapSystem(MapGenerator& mapGenerator, ce::EventBus* eventBus)
	: m_mapGenerator{ mapGenerator }, m_tilemap{ nullptr }, m_eventBus{ eventBus }
{ 
}

void MapSystem::update(ce::SystemUpdateContext& context)
{
	// player or camera? 
	auto view = context.registry.view<ce::CameraComponent>();
	
	auto activeCamera = view.findFirst([](const cursed_engine::CameraComponent& cameraComponent)
		{
			return cameraComponent.isActive;
		});

	//cursed_engine::FVec2 worldPosition{}; // create FVec2::zero();??
	ce::Bounds cameraBounds{};

	if (activeCamera.has_value())
	{
		const auto& cameraTransformComponent = context.registry.getComponent<ce::TransformComponent>(activeCamera.value());
		//worldPosition = cameraTransformComponent.position;
		cameraBounds = context.registry.getComponent<ce::CameraComponent>(activeCamera.value()).bounds;
	}

	// FIX THIS! NOT SO GOOD TO USE FRECT (CONTAINS WIDTH AND HEIGHT, but used as positions....)
	// for each corner 
	std::array<ce::FVec2, 4> corners
	{
		ce::FVec2{ cameraBounds.min.x, cameraBounds.min.y }, // top left
		ce::FVec2{ cameraBounds.max.x, cameraBounds.min.y }, // top right
		ce::FVec2{ cameraBounds.min.x, cameraBounds.max.y }, // bottom left
		ce::FVec2{ cameraBounds.max.x, cameraBounds.max.y } // bottom right
	};

	for (const ce::FVec2& corner : corners)
	{
		const auto& [xCoord, yCoord] = getMapChunkCoordinatesFromWorldPosition(corner);

		if (!m_tilemap->isValidChunk(xCoord, yCoord))
		{
			ce::Logger::logInfo(std::format("Not valid chunk! Coords: {}, {}", xCoord, yCoord));

			// generate new mapchunk
			auto mapChunk = m_mapGenerator.generateMapChunk(xCoord, yCoord);
			m_tilemap->insertMapChunk(std::move(mapChunk));

			m_eventBus->publishInstantly<MapChunkCreatedEvent>(xCoord, yCoord);
		}
	}

	//const auto& [xCoord, yCoord] = getMapChunkCoordinatesFromWorldPosition(worldPosition);
	////cursed_engine::Logger::logInfo(std::format("Coords: {}, {}", xCoord, yCoord));
	//
	//if (!m_tilemap->isValidChunk(xCoord, yCoord))
	//{
	//	// generate new mapchunk
	//	auto mapChunk = m_mapGenerator.generateMapChunk(xCoord, yCoord);
	//	m_tilemap->insertMapChunk(std::move(mapChunk));

	//	m_eventBus->publishInstantly<MapChunkCreatedEvent>(xCoord, yCoord);
	//}

	//auto* chunk = m_tilemap->getChunkAtPosition((int)worldPosition.x, (int)worldPosition.y); // pass floats?
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
	//	// m_tilemap->insertMapChunk(std::move(mapChunk));
	//}
	// check if needing to generate new chunk...
	// 
	// m_mapGenerator.

}

void MapSystem::setTilemap(Tilemap* map)
{
	m_tilemap = map;
}