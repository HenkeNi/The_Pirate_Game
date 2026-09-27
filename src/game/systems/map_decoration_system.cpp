#include "game/systems/map_decoration_system.h"
#include "game/events/events.h"
#include "game/map/tilemap.h"
#include "game/map/tile_registry.h"
#include <engine/core/events/event_bus.h>
#include <engine/utils/random/random.h>
#include <engine/ecs/component/core_components.h>
#include <engine/ecs/entity/entity_factory.h>
#include <engine/core/logger.h>

MapDecorationSystem::MapDecorationSystem(cursed_engine::EntityFactory& factory, cursed_engine::EventBus& eventBus)
	: m_tilemap{ nullptr }, m_entityFactory{ factory }, m_eventBus{ eventBus }
{
	m_eventBus.subscribe<MapChunkCreatedEvent>(
		[&](const MapChunkCreatedEvent& e)
		{
			if (!m_tilemap)
			{
				cursed_engine::Logger::logError("[MapDecorationSystem::OnMapChunkCreatedEvent] - No tilemap set!");
				return;
			}

			if (!m_tileset)
			{
				cursed_engine::Logger::logError("[MapDecorationSystem::OnMapChunkCreatedEvent] - No valid tileset!");
				return;
			}

			assert(m_tileset && m_tilemap && "Invalid map data!");

			if (const MapChunk* mapChunk = m_tilemap->getChunkAtCoords(e.x, e.y))
			{
				//const auto* tileset = m_tileRegistry.getTileset("island_tileset"); // fix hardcoded set

				const auto& groundLayer = mapChunk->layers.at(1);
				//for (const auto& tileId : groundLayer.tileIds)
				for (int i = 0; i < groundLayer.tileIds.size(); ++i)
				{
					const auto& tileId = groundLayer.tileIds[i];

					const auto& tileType = m_tileset->tileTypes.at(tileId);

					for (const auto& spawnable : tileType.spawnables)
					{
						if (cursed_engine::random::generateRandomFloatingPoint(0.f, 1.f) < spawnable.chance)
						{
							ce::FVec2 position = getTileWorldPosition(mapChunk->coords.x, mapChunk->coords.y, i);
							
							auto entityHandle = m_entityFactory.createFromPrefab(spawnable.id, position);
							//auto entityHandle = m_entityFactory.createFromPrefab("palm_tree");
							//if (entityHandle.has_value())
							//{
							//	//auto& transformComponent = entityHandle.value().getComponent<cursed_engine::TransformComponent>();

							//	// TEST
							//	//auto& spriteComponent = entityHandle.value().getComponent<cursed_engine::SpriteComponent>();
							//	//if (!spriteComponent.atlasHandle.isValid())
							//	//{
							//	//	int x = 20;

							//	//}
							//		// set atlasregion???

							//	//if (spriteComponent.atlasRegion.rect.w < 1 || spriteComponent.atlasRegion.rect.h < 1)
							//	//{
							//	//	int x = 20;
							//	//}
							//}
							break;
						}
					}
				}

				// for each tile...


				// TODO; get current tile, check spawnables
				//tileSet->tileTypes.
				int x = 20;

				// TODO; update tilemap with occupied or not? 
			}

			//for (const auto& tile : m_tilemap)
			// for each tile, check tile type... small change for tree, rock, etc...
		});
}

void MapDecorationSystem::setTilemap(Tilemap* map)
{
	m_tilemap = map;
}

void MapDecorationSystem::setTileset(const Tileset* tileset)
{
	m_tileset = tileset;
}