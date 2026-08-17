#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class EventBus;
}

class TileMap;
class MapGenerator;

// mark chunks dirty? generate new? serialize? deserialize?
class MapSystem : public cursed_engine::System
{
public:
	MapSystem(MapGenerator& mapGenerator, cursed_engine::EventBus* eventBus);

	void update(cursed_engine::SystemContext& context) override;
	void setMap(TileMap* map);

private:
	MapGenerator& m_mapGenerator;
	TileMap* m_tileMap;
	cursed_engine::EventBus* m_eventBus;
};