#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class EventBus;
	class EntityFactory;
}

class Tilemap;
struct Tileset;

//class TileRegistry;
//class AssetManager;

class MapDecorationSystem : public cursed_engine::UpdateSystem
{
public:
	MapDecorationSystem(cursed_engine::EntityFactory& factory, cursed_engine::EventBus& eventBus);

	void setTilemap(Tilemap* map);
	void setTileset(const Tileset* tileset);

private:
	//TileRegistry& m_tileRegistry;
	Tilemap* m_tilemap;
	const Tileset* m_tileset;

	cursed_engine::EntityFactory& m_entityFactory;
	cursed_engine::EventBus& m_eventBus;
};