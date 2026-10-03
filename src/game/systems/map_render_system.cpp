#include "game/systems/map_render_system.h"
#include "game/components/components.h"
#include "game/map/tile_registry.h"
#include "game/map/tilemap.h"
#include <engine/ecs/ecs_registry.h>
#include <engine/ecs/component/core_components.h>
#include <format>

using cursed_engine::FVec2;

MapRenderSystem::MapRenderSystem(cursed_engine::RenderAPI renderAPI, cursed_engine::TextureManager* textureManager)
	: m_renderAPI{ renderAPI }, m_textureManager{ textureManager }, m_tilemap{ nullptr }, m_tileset{ nullptr }
{
}

void MapRenderSystem::render(cursed_engine::SystemRenderContext& context)
{
	//assert(m_tilemap && m_tileset && "Not valid map data");

	if (m_tilemap && m_tileset) // do early return instead
	{
		/*	auto view = context.registry.view<cursed_engine::CameraComponent>();
			auto activeCamera = view.findFirst([](const cursed_engine::CameraComponent& cameraComponent)
				{
					return cameraComponent.isActive;
				});*/



				// TEMP -> figure out best way to get texture...  get texture from tileset? or tilemap?
		auto handle = m_textureManager->getHandle(cursed_engine::TextureDescriptor{ "../assets/textures/map/island_tileset.png" });
		//const auto& texture = m_textureManager->get(handle); /////////////////////// HAVIN RENDER BACKEND FRIEND CLASS WOULD MAKE THIS BE ABLE TO BE CONST!
		auto& texture = m_textureManager->get(handle); // TODO; make const!

		//m_tileset->textureSize.x = texture->getWidth();
		//m_tileset->textureSize.y = texture->getHeight();

		auto test = m_tilemap->getVisibleMapChunks();

		for (auto* mapChunk : m_tilemap->getVisibleMapChunks())
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
					assert(m_tileset && "Not a valid tileset!");

					buildMapChunkGeometry(getWorldPosition(*mapChunk), layer, *m_tileset, { texture.getWidth() ,  texture.getHeight() }); // dont pass tielset?!!
					layer.isDirty = false;
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
					worldPosition = cameraTransformComponent.position; //worldPosition = cameraTransformComponent.position + cameraTransformComponent.pivot; //  USE PIVOT?

					//updateMapChunkPosition(layer, worldPosition);
					m_renderAPI.setRenderState(cursed_engine::RenderState{ cursed_engine::View{worldPosition, 0.0, 0.f}, cursed_engine::Projection{ { 1280, 720 } } }); // view, projection				
				}



				// TODO; need to know what texture the layer is using...

				m_renderAPI.drawGeometry(layer.geometry, texture);
			}
		}
	}

	//renderTest();
}

void MapRenderSystem::setTilemap(Tilemap* tilemap)
{
	m_tilemap = tilemap;
}

void MapRenderSystem::setTileset(const Tileset* tileset)
{
	m_tileset = tileset;
}

void MapRenderSystem::buildMapChunkGeometry(const cursed_engine::IVec2& position, TileLayer& tileLayer, const Tileset& tileset, const cursed_engine::IVec2& size)
{
	auto& geometry = tileLayer.geometry; // pass in geometry instead? (can set isDirty)

	// Dont clear - just overwrite?
	// reserv or resize vertices?

	auto& vertices = geometry.vertices;
	vertices.clear();

	auto& indices = geometry.indices;
	indices.clear();

	
	// maybe this is wrong?
	for (int y = 0; y < TileLayer::height; y++)
	{
		for (int x = 0; x < TileLayer::width; x++)
		{
			int tileIndex = x + y * TileLayer::width;

			TileId tileId = tileLayer.tileIds.at(tileIndex); // returns the number of the tile

			cursed_engine::UVRect uvRect;

			float tileSize = (float)map_constants::TILE_SIZE; // do above loops? read from tileset?

			// TODO; create function? apply texture?
			if (auto it = tileset.tileTypes.find(tileId); it != tileset.tileTypes.end())
			{
				cursed_engine::IVec2 coords = it->second.atlasCoord;

				uvRect.u0 = (coords.x * tileSize) / size.x; //tileset.textureSize.x;
				uvRect.v0 = (coords.y * tileSize) / size.y; //tileset.textureSize.y;

				uvRect.u1 = ((coords.x + 1) * tileSize) / size.x;// tileset.textureSize.x;
				uvRect.v1 = ((coords.y + 1) * tileSize) / size.y;// tileset.textureSize.y;
			}
			else
			{
				assert(false && "Tile Id not found!");
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
