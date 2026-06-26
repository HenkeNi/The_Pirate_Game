#include "game/systems/map_render_system.h"
#include "game/components/components.h"
#include "game/map/tile_map.h"

using cursed_engine::FVec2;

MapRenderSystem::MapRenderSystem(cursed_engine::RenderAPI renderAPI, cursed_engine::TextureManager* textureManager, TileRegistry& registry)
	: m_renderAPI{ renderAPI }, m_textureManager{ textureManager }, m_tileRegistry{ registry }, m_tileMap{ nullptr }
{
}

void MapRenderSystem::update(cursed_engine::SystemContext& context)
{
	if (m_tileMap)
	{
		// TEMP -> figure out best way to get texture...  get texture from tileset? or tilemap?
		auto handle = m_textureManager->getHandle(cursed_engine::TextureDescriptor{ "../assets/textures/map/island_tileset.png" });
		auto* texture = m_textureManager->get(handle);

		for (auto* mapChunk : m_tileMap->getVisibleMapChunks())
		{
			assert(mapChunk && "Not valid mapChunk");
			if (!mapChunk)
			{
				cursed_engine::Logger::logError("Invalid MapChunk");
				continue;
			}

			if (mapChunk->isDirty)
				buildMapChunkGeometry(*mapChunk); // each chunk has its own geometry? or create a "big one"?

			m_renderAPI.drawGeometry(mapChunk->geometry, *texture);
		}
		/*for (auto& chunk : tileMap.GetVisibleChunks(camera))
		{
			if (chunk.Dirty)
				chunk.RebuildGeometry(tileRegistry);

			renderer.DrawGeometry(chunk.Geometry);
		}*/





		// who is responsbile for updating dirty chunks? MapRenderSystem? MapSystem? scene updates ´map`??

		// if chunk is dirty? or mapsystem does it?

		//  render chunk by chunk?
		// get tiles... -

		// 


		int x = 20;
	}

	//renderTest();
	
	int x = 20;
}

void MapRenderSystem::renderTest()
{
	cursed_engine::Geometry geometry;
	auto& vert = geometry.vertices;
	vert.resize(4);

	const int textureWidth = 2048;
	const int textureHeight = 768;

	const int columns = 16;
	const int rows = 6;

	const int tileWidth = textureWidth / columns;
	const int tileHeight = textureHeight / rows;

	//float uSize = 128.0f / 2048.0f; // 0.0625
	//float vSize = 128.0f / 768.0f;  // 0.16666667

	int column = 3;
	int row = 2;

	float u0 = (column * 128.0f) / 2048.0f;
	float v0 = (row * 128.0f) / 768.0f;

	float u1 = ((column + 1) * 128.0f) / 2048.0f;
	float v1 = ((row + 1) * 128.0f) / 768.0f;

	vert[0].position.x = 100;
	vert[0].position.y = 100;
	vert[0].color.r = 1;
	vert[0].color.g = 1;
	vert[0].color.b = 1;
	vert[0].color.a = 1;
	vert[0].uv = { u0, v0 };

	// left
	vert[1].position.x = 200;
	vert[1].position.y = 100;
	vert[1].color.r = 1;
	vert[1].color.g = 1;
	vert[1].color.b = 1;
	vert[1].color.a = 1;
	vert[1].uv = { u1, v0 };

	// right
	vert[2].position.x = 100;
	vert[2].position.y = 200;
	vert[2].color.r = 1;
	vert[2].color.g = 1;
	vert[2].color.b = 1;
	vert[2].color.a = 1;
	vert[2].uv = { u0, v1 };

	vert[3].position.x = 200;
	vert[3].position.y = 200;
	vert[3].color.r = 1;
	vert[3].color.g = 1;
	vert[3].color.b = 1;
	vert[3].color.a = 1;
	vert[3].uv = { u1, v1 };

	//vert[4].position.x = 800;
	//vert[4].position.y = 950;
	//vert[4].color.r = 1;
	//vert[4].color.g = 1;
	//vert[4].color.b = 1;
	//vert[4].color.a = 1;


	geometry.indices = {
	0, 1, 2,
	1, 3, 2
	};

	auto handle = m_textureManager->getHandle(cursed_engine::TextureDescriptor{ "../assets/textures/map/island_tileset.png" });

	auto* texture = m_textureManager->get(handle);
	m_renderAPI.drawGeometry(geometry, *texture);
}

void MapRenderSystem::setTileMap(TileMap* tileMap)
{
	m_tileMap = tileMap;
}

void MapRenderSystem::buildMapChunkGeometry(MapChunk& mapChunk)
{
	auto& geometry = mapChunk.geometry; // pass in geometry instead? (can set isDirty)

	// Dont clear - just overwrite?
	auto& vertices = geometry.vertices;
	vertices.clear();

	auto& indices = geometry.indices;
	indices.clear();

	static int TILE_SIZE = 10; // TEMP!

	// reserv or resize vertices?

	for (int y = 0; y < map_constants::MAP_CHUNK_HEIGHT; y++)
		for (int x = 0; x < map_constants::MAP_CHUNK_WIDTH; x++)
		{
			//int tileIndex = x + y * map_constants::MAP_CHUNK_WIDTH;
			//auto index = mapChunk.tileIds.at(tileIndex);//  might be wrong... // correct?


			// const Tile& tile = chunk.tiles[tileIndex]; -- use tile id instead...

			////////////////////////////////7
			int column = 3;
			int row = 2;

			float u0 = (column * 128.0f) / 2048.0f;
			float v0 = (row * 128.0f) / 768.0f;

			float u1 = ((column + 1) * 128.0f) / 2048.0f;
			float v1 = ((row + 1) * 128.0f) / 768.0f;


			////////////////////////////////////

			//std::size_t baseVertex = vertices.size();

			/*float px = x * TILE_SIZE;
			float py = y * TILE_SIZE;*/
			int px = x * TILE_SIZE;
			int py = y * TILE_SIZE;

			// 4 vertices (quad)
			vertices.emplace_back(FVec2{ (float)px, (float)py },							FVec2{ u0, v0 });
			vertices.emplace_back(FVec2{ float(px + TILE_SIZE), (float)py },				FVec2{ u1, v0 });
			vertices.emplace_back(FVec2{ (float)px, float(py + TILE_SIZE) },				FVec2{ u0, v1 });
			vertices.emplace_back(FVec2{ (float)px + TILE_SIZE, float(py + TILE_SIZE) },	FVec2{ u1, v1 });

			//vertices.push_back({ {px, py},                uv0, tile.color });
			//vertices.push_back({ {px + TILE_SIZE, py},    tile.uv1, tile.color });
			//vertices.push_back({ {px, py + TILE_SIZE},    tile.uv2, tile.color });
			//vertices.push_back({ {px + TILE_SIZE, py + TILE_SIZE}, tile.uv3, tile.color }); */

			int baseVertex = (int)vertices.size();

			// First triangle
			indices.push_back(baseVertex + 0);
			indices.push_back(baseVertex + 1);
			indices.push_back(baseVertex + 2);

			// Second triangle
			indices.push_back(baseVertex + 2);
			indices.push_back(baseVertex + 1);
			indices.push_back(baseVertex + 3);
		}

	mapChunk.isDirty = false; // do last!


	// store map chunk vertices in the MapChunkComponent OR in some registry?
/*
	if (!mapChunkComponent.isDirty)
		return;

	auto& vertices = mapChunkComponent.geometry.vertices;
	vertices.clear();

	auto& indices = mapChunkComponent.geometry.indices;
	indices.clear();

	static int TILE_SIZE = 10;*/ // TEMP!

	//for (int y = 0; y < MapChunkComponent::height; y++)
	//	for (int x = 0; x < MapChunkComponent::width; x++)
	//	{
	//		int tileIndex = x + y * MapChunkComponent::width;
	//		// const Tile& tile = chunk.tiles[tileIndex]; -- use tile id instead...

	//		int baseVertex = vertices.size();

	//		float px = x * TILE_SIZE;
	//		float py = y * TILE_SIZE;

	//		// 4 vertices (quad)
	//		vertices.push_back({ {px, py},                tile.uv0, tile.color });
	//		vertices.push_back({ {px + TILE_SIZE, py},    tile.uv1, tile.color });
	//		vertices.push_back({ {px, py + TILE_SIZE},    tile.uv2, tile.color });
	//		vertices.push_back({ {px + TILE_SIZE, py + TILE_SIZE}, tile.uv3, tile.color });

	//		// 6 indices (2 triangles)
	//		indices.push_back(baseVertex + 0);
	//		indices.push_back(baseVertex + 1);
	//		indices.push_back(baseVertex + 2);

	//		indices.push_back(baseVertex + 2);
	//		indices.push_back(baseVertex + 1);
	//		indices.push_back(baseVertex + 3);
	//	}
}