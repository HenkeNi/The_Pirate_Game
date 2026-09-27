#pragma once
#include <engine/ecs/system/system.h>
#include <engine/rendering/render_api.h>
#include <engine/rendering/render_types.h>
#include <engine/resources/resource_types.h>

namespace cursed_engine
{
	class RenderAPI;
}

//- Rebuild dirty chunk geometry
//- Frustum / viewport culling
//- renderer.DrawGeometry(...)

class TileRegistry;
class Tilemap;
struct Tileset;
struct TileLayer;

// cull chunks? 
class MapRenderSystem : public cursed_engine::RenderSystem
{
public:
	MapRenderSystem(cursed_engine::RenderAPI renderAPI, cursed_engine::TextureManager* textureManager);

	void render(cursed_engine::SystemRenderContext& context) override;

	void setTilemap(Tilemap* tilemap);
	void setTileset(const Tileset* tileset);

private:
	//void renderTest();
	void buildMapChunkGeometry(const cursed_engine::IVec2& position, TileLayer& tileLayer, const Tileset& tileset, const cursed_engine::IVec2& size); // rename? or rework? not mesh but geometry...
	//void updateMapChunkPosition(TileLayer& tileLayer, const cursed_engine::FVec2& cameraPos); // or do in render backend?

	cursed_engine::RenderAPI m_renderAPI;
	cursed_engine::TextureManager* m_textureManager;

	const Tileset* m_tileset;
	//TileRegistry& m_tileRegistry;
	Tilemap* m_tilemap; // references tileset?
};