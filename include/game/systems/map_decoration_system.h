#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class EventBus;
	class EntityFactory;
}

class TileMap;
class TileRegistry;

class MapDecorationSystem : public cursed_engine::System
{
public:
	MapDecorationSystem(TileRegistry& registry, cursed_engine::EntityFactory& factory, cursed_engine::EventBus& eventBus);

	void setTileMap(TileMap* map);

private:
	TileRegistry& m_tileRegistry;
	TileMap* m_tileMap;

	cursed_engine::EntityFactory& m_entityFactory;
	cursed_engine::EventBus& m_eventBus;
};