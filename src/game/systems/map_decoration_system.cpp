#include "game/systems/map_decoration_system.h"
#include "game/events/events.h"
#include "game/map/tile_map.h"
#include "game/map/tile_registry.h"
#include <engine/core/events/event_bus.h>
#include <engine/utils/random/random.h>
#include <engine/ecs/entity/entity_factory.h>
#include <engine/core/logger.h>

MapDecorationSystem::MapDecorationSystem(TileRegistry& registry, cursed_engine::EntityFactory& factory, cursed_engine::EventBus& eventBus)
	: m_tileRegistry{ registry }, m_tileMap{ nullptr }, m_entityFactory{ factory }, m_eventBus{ eventBus }
{
	m_eventBus.subscribe<MapChunkCreatedEvent>(
		[&](const MapChunkCreatedEvent& e)
		{
			if (!m_tileMap)
			{
				cursed_engine::Logger::logError("[MapDecorationSystem::OnMapChunkCreatedEvent] - No tilemap set!");
				return;
			}

			if (const MapChunk* mapChunk = m_tileMap->getChunkAtCoords(e.x, e.y))
			{
				const auto* tileSet = m_tileRegistry.getTileSet("island_tileset"); // fix hardcoded set

				const auto& groundLayer = mapChunk->layers.at(1);
				for (const auto& tileId : groundLayer.tileIds)
				{
					const auto& tileType = tileSet->tileTypes.at(tileId);

					for (const auto& spawnable : tileType.spawnables)
					{
						if (cursed_engine::random::generateRandomFloatingPoint(0.f, 1.f) < spawnable.chance)
						{
							auto entity = m_entityFactory.createFromPrefab(spawnable.id);
							break;
						}
						int x = 20;
					}
				}

				// for each tile...

				
				// TODO; get current tile, check spawnables
				//tileSet->tileTypes.
				int x = 20;

				// TODO; update tilemap with occupied or not? 
			}

			//for (const auto& tile : m_tileMap)
			// for each tile, check tile type... small change for tree, rock, etc...
		});
}

void MapDecorationSystem::setTileMap(TileMap* map)
{
	m_tileMap = map;
}