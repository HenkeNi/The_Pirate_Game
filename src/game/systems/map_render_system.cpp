#include "game/systems/map_render_system.h"
#include "game/components/components.h"
#include "game/map/tile_map.h"
#include "game/map/tile_registry.h"
#include <engine/ecs/ecs_registry.h>
#include <engine/ecs/component/core_components.h>
#include <format>

using cursed_engine::FVec2;

MapRenderSystem::MapRenderSystem(cursed_engine::RenderAPI renderAPI, cursed_engine::TextureManager* textureManager, TileRegistry& registry)
	: m_renderAPI{ renderAPI }, m_textureManager{ textureManager }, m_tileRegistry{ registry }, m_tileMap{ nullptr }
{
}

void MapRenderSystem::update(cursed_engine::SystemContext& context)
{
	if (m_tileMap)
	{
		/*	auto view = context.registry.view<cursed_engine::CameraComponent>();
			auto activeCamera = view.findFirst([](const cursed_engine::CameraComponent& cameraComponent)
				{
					return cameraComponent.isActive;
				});*/



				// TEMP -> figure out best way to get texture...  get texture from tileset? or tilemap?
		auto handle = m_textureManager->getHandle(cursed_engine::TextureDescriptor{ "../assets/textures/map/island_tileset.png" });
		auto* texture = m_textureManager->get(handle);

		auto test = m_tileMap->getVisibleMapChunks();

		for (auto* mapChunk : m_tileMap->getVisibleMapChunks())
		{
			assert(mapChunk && "Not a valid MapChunk!");
			if (!mapChunk)
			{
				cursed_engine::Logger::logError("[MapRenderSystem::update] - Not a valid MapChunk!");
				continue;
			}

			// move into the function for constructing map chunk gemometry?
			for (auto& layer : mapChunk->layers)
			{
				if (!layer.isActive)
					continue;

				if (layer.isDirty)
				{
					if (auto* tileSet = m_tileRegistry.getTileSet(layer.tileSetId)) [[likely]]
					{
						buildMapChunkGeometry(getWorldPosition(*mapChunk), layer, *tileSet);
						layer.isDirty = false;
					}
					else
					{
						cursed_engine::Logger::logError("[MapRenderSystem::update] - Invalid TileSet!");
						assert(false && "Invalid TileSet - can't continue rendering");
						continue;
					}
				}


				// TODO; do  before loop instead... - not every frame!!!
				// update chunk position - TODO, mabe only do if havent built geomtry this frame (pass in camera pos to build geometry)

				auto view = context.registry.view<cursed_engine::CameraComponent>();
				auto activeCamera = view.findFirst([](const cursed_engine::CameraComponent& cameraComponent)
					{
						return cameraComponent.isActive;
					});

				FVec2 worldPosition{}; // create FVec2::zero();??

				if (activeCamera.has_value())
				{
					const auto& cameraTransformComponent = context.registry.getComponent<cursed_engine::TransformComponent>(activeCamera.value());
					worldPosition = cameraTransformComponent.position;

					//updateMapChunkPosition(layer, worldPosition);
					m_renderAPI.setRenderState(cursed_engine::RenderState{ cursed_engine::View{worldPosition, 0.0, 0.f}, cursed_engine::Projection{ { 1280, 720 } } }); // view, projection				
				}



				// TODO; need to know what texture the layer is using...

				m_renderAPI.drawGeometry(layer.geometry, *texture);
			}
		}
	}

	//renderTest();
}

//void MapRenderSystem::renderTest()
//{
//	cursed_engine::Geometry geometry;
//	auto& vert = geometry.vertices;
//	vert.resize(4);
//
//	const int textureWidth = 2048;
//	const int textureHeight = 768;
//
//	const int columns = 16;
//	const int rows = 6;
//
//	const int tileWidth = textureWidth / columns;
//	const int tileHeight = textureHeight / rows;
//
//	//float uSize = 128.0f / 2048.0f; // 0.0625
//	//float vSize = 128.0f / 768.0f;  // 0.16666667
//
//	int column = 3;
//	int row = 2;
//
//	float u0 = (column * 128.0f) / 2048.0f;
//	float v0 = (row * 128.0f) / 768.0f;
//
//	float u1 = ((column + 1) * 128.0f) / 2048.0f;
//	float v1 = ((row + 1) * 128.0f) / 768.0f;
//
//	vert[0].position.x = 100;
//	vert[0].position.y = 100;
//	vert[0].color.r = 1;
//	vert[0].color.g = 1;
//	vert[0].color.b = 1;
//	vert[0].color.a = 1;
//	vert[0].uv = { u0, v0 };
//
//	// left
//	vert[1].position.x = 200;
//	vert[1].position.y = 100;
//	vert[1].color.r = 1;
//	vert[1].color.g = 1;
//	vert[1].color.b = 1;
//	vert[1].color.a = 1;
//	vert[1].uv = { u1, v0 };
//
//	// right
//	vert[2].position.x = 100;
//	vert[2].position.y = 200;
//	vert[2].color.r = 1;
//	vert[2].color.g = 1;
//	vert[2].color.b = 1;
//	vert[2].color.a = 1;
//	vert[2].uv = { u0, v1 };
//
//	vert[3].position.x = 200;
//	vert[3].position.y = 200;
//	vert[3].color.r = 1;
//	vert[3].color.g = 1;
//	vert[3].color.b = 1;
//	vert[3].color.a = 1;
//	vert[3].uv = { u1, v1 };
//
//	//vert[4].position.x = 800;
//	//vert[4].position.y = 950;
//	//vert[4].color.r = 1;
//	//vert[4].color.g = 1;
//	//vert[4].color.b = 1;
//	//vert[4].color.a = 1;
//
//
//	geometry.indices = {
//		0, 1, 2,
//		1, 3, 2
//	};
//
//	auto handle = m_textureManager->getHandle(cursed_engine::TextureDescriptor{ "../assets/textures/map/island_tileset.png" });
//
//	auto* texture = m_textureManager->get(handle);
//	m_renderAPI.drawGeometry(geometry, *texture);
//}

void MapRenderSystem::setTileMap(TileMap* tileMap)
{
	m_tileMap = tileMap;
}

void MapRenderSystem::buildMapChunkGeometry(const cursed_engine::IVec2& position, TileLayer& tileLayer, const TileSet& tileSet)
{
	auto& geometry = tileLayer.geometry; // pass in geometry instead? (can set isDirty)

	// Dont clear - just overwrite?
	// reserv or resize vertices?

	auto& vertices = geometry.vertices;
	vertices.clear();

	auto& indices = geometry.indices;
	indices.clear();

	for (int y = 0; y < TileLayer::height; y++)
	{
		for (int x = 0; x < TileLayer::width; x++)
		{
			int tileIndex = x + y * TileLayer::width;

			TileId tileId = tileLayer.tileIds.at(tileIndex); // returns the number of the tile

			cursed_engine::UVRect uvRect;

			float tileSize = (float)map_constants::TILE_SIZE; // do above loops? read from tileset?

			// TODO; create function? apply texture?
			if (auto it = tileSet.tileTypes.find(tileId); it != tileSet.tileTypes.end())
			{
				cursed_engine::IVec2 coords = it->second.atlasCoord;

				uvRect.u0 = (coords.x * tileSize) / tileSet.textureSize.x;
				uvRect.v0 = (coords.y * tileSize) / tileSet.textureSize.y;

				uvRect.u1 = ((coords.x + 1) * tileSize) / tileSet.textureSize.x;
				uvRect.v1 = ((coords.y + 1) * tileSize) / tileSet.textureSize.y;
			}
			else
			{
				// TODO; use some error texture?
			}

			// TODO; using tileId figure out what 
			// TODO; figure out which atlas coordinates should be used...

			//	m_tileRegistry.get();

			//auto index = mapChunk.tileIds.at(tileIndex);//  might be wrong... // correct?
			// const Tile& tile = chunk.tiles[tileIndex]; -- use tile id instead...

			////////////////////////////////7
			/*int column = 1;
			int row = 1;

			uvRect.u0 = (column * 128.0f) / 2048.0f;
			uvRect.v0 = (row * 128.0f) / 768.0f;

			uvRect.u1 = ((column + 1) * 128.0f) / 2048.0f;
			uvRect.v1 = ((row + 1) * 128.0f) / 768.0f;*/

			////////////////////////////////////

			// or maybe h should be py - tile size?
			// what happens if x is negative? -1 * 32 = -32 -> could tryy placing start area at negative coords!
			// print player pos?

			const int px = (x * map_constants::TILE_SIZE) + position.x;
			const int py = (y * map_constants::TILE_SIZE) + position.y;

			//cursed_engine::Logger::logInfo(std::format("{}, {}, {}, {},", (float)px, float(px + map_constants::TILE_SIZE), (float)py, float(py + map_constants::TILE_SIZE)));

			int baseVertex = (int)vertices.size();

			const auto& [u0, v0, u1, v1] = uvRect;

			// 4 vertices (quad)
			vertices.emplace_back(FVec2{ (float)px, (float)py }, FVec2{ u0, v0 });
			vertices.emplace_back(FVec2{ float(px + map_constants::TILE_SIZE), (float)py }, FVec2{ u1, v0 });
			vertices.emplace_back(FVec2{ (float)px, float(py + map_constants::TILE_SIZE) }, FVec2{ u0, v1 });
			vertices.emplace_back(FVec2{ (float)px + map_constants::TILE_SIZE, float(py + map_constants::TILE_SIZE) }, FVec2{ u1, v1 });

			// First triangle
			indices.push_back(baseVertex + 0);
			indices.push_back(baseVertex + 1);
			indices.push_back(baseVertex + 2);

			// Second triangle
			indices.push_back(baseVertex + 2);
			indices.push_back(baseVertex + 1);
			indices.push_back(baseVertex + 3);
		}
	}
}

//void MapRenderSystem::updateMapChunkPosition(TileLayer& tileLayer, const cursed_engine::FVec2& cameraPos)
//{
//	/*auto& vertices = tileLayer.geometry.vertices;
//
//	for (int i = 0; i < vertices.size(); ++i)
//	{
//		vertices[i].position -= cameraPos;
//	} */
//
//	//for (int y = 0; y < TileLayer::height; y++)
//	//{
//	//	for (int x = 0; x < TileLayer::width; x++)
//	//	{
//	//		int tileIndex = x + y * TileLayer::width;
//
//	//		float tileSize = (float)map_constants::TILE_SIZE; // do above loops? read from tileset?
//
//	//		const int px = x * map_constants::TILE_SIZE; // check data type..
//	//		const int py = y * map_constants::TILE_SIZE;
//
//	//		auto& geometry = tileLayer.geometry;
//
//	//		auto& vertex = geometry.vertices[tileIndex];
//
//	//		vertex.position 
//
//	//		// 4 vertices (quad)
//	//		vertices.emplace_back(FVec2{ (float)px, (float)py }, FVec2{ u0, v0 });
//	//		vertices.emplace_back(FVec2{ float(px + map_constants::TILE_SIZE), (float)py }, FVec2{ u1, v0 });
//	//		vertices.emplace_back(FVec2{ (float)px, float(py + map_constants::TILE_SIZE) }, FVec2{ u0, v1 });
//	//		vertices.emplace_back(FVec2{ (float)px + map_constants::TILE_SIZE, float(py + map_constants::TILE_SIZE) }, FVec2{ u1, v1 });
//
//
//	//	}
//	//}
//}
