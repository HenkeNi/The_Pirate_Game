#pragma once
#include <engine/ecs/system/system.h>
#include <engine/rendering/render_api.h>
#include <engine/resources/resource_types.h>

namespace cursed_engine
{
	class RenderAPI;
}

//- Rebuild dirty chunk geometry
//- Frustum / viewport culling
//- renderer.DrawGeometry(...)

class TileMap;
class TileRegistry;
struct TileLayer;
struct TileSet;

// cull chunks? 
class MapRenderSystem : public cursed_engine::System
{
public:
	MapRenderSystem(cursed_engine::RenderAPI renderAPI, cursed_engine::TextureManager* textureManager, TileRegistry& registry);

	void update(cursed_engine::SystemContext& context) override;

	void setTileMap(TileMap* tileMap);

private:
	void renderTest();
	void buildMapChunkGeometry(TileLayer& tileLayer, const TileSet& tileSet); // rename? or rework? not mesh but geometry...

	cursed_engine::RenderAPI m_renderAPI;
	cursed_engine::TextureManager* m_textureManager;

	TileRegistry& m_tileRegistry;
	TileMap* m_tileMap;
};