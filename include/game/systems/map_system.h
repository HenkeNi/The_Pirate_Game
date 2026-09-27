#pragma once
#include <engine/ecs/system/system.h>

namespace cursed_engine
{
	class EventBus;
}

namespace ce = cursed_engine; // DONT DO IN EVERY FILE!! -> put in aliases.h (or something) namespace_aliases.h or  put in precompiled header!

class Tilemap;
class MapGenerator;

// mark chunks dirty? generate new? serialize? deserialize?
class MapSystem : public ce::UpdateSystem
{
public:
	MapSystem(MapGenerator& mapGenerator, ce::EventBus* eventBus);

	void update(ce::SystemUpdateContext& context) override;
	void setTilemap(Tilemap* map);

private:
	MapGenerator& m_mapGenerator;
	Tilemap* m_tilemap;
	ce::EventBus* m_eventBus;
};