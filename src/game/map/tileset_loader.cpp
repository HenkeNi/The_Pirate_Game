#include "game/map/tileset_loader.h"
#include <engine/assets/asset_manager.h>
#include <engine/utils/json/json_document.h>
#include <engine/utils/json/json_value.h>
#include <engine/core/logger.h>
#include <engine/core/result.h>
#include <format>

TilesetLoader::TilesetLoader(cursed_engine::AssetManager& assetManager)
	: m_assetManager{ assetManager }
{
}

ce::Result<Tileset> TilesetLoader::load(const std::filesystem::path& path) const
{
	cursed_engine::JsonDocument doc;
	ce::Result<void> result = doc.loadFromFile(path);

	if (!result.ok())
	{
		return ce::Result<Tileset>::failure(std::format("Failed to load tile types from: {}, reason: {}", path.string(), result.message()));
	}

	Tileset tileset;
	//tileset.name = doc["name"].asString();

	std::string atlasId = doc["atlas_id"].asString();
	//std::string atlasId = doc["texture_id"].asString();
	auto handle = m_assetManager.getAssetHandle<cursed_engine::TextureAtlas>(atlasId);

	if (!handle.isValid())
	{
		// Log error?
		return ce::Result<Tileset>::failure("Failed to fetch TextureAtlas handle!");
	}

	// set.textureHandle = handle; textur ehandle or texture atlas handle?

	/*set.textureSize.x = width;
	set.textureSize.y = height;*/


	for (const auto& tile : doc["tiles"].asArray())
	{
		TileDefinition definition;
		definition.layer = tile["layer"].asInt();
		definition.walkable = tile["walkable"].asBool();
		definition.atlasCoord.x = tile["column"].asInt(); // index instead?
		definition.atlasCoord.y = tile["row"].asInt();
		// calculate sprite index...

		int id = tile["id"].asInt();
		std::string name = tile["name"].asString();

		definition.id = id; // ??
		definition.spriteIndex = 0; // ???

		if (tile.hasMember("spawnables"))
		{
			for (const auto& spawnable : tile["spawnables"].asArray())
			{
				std::string id = spawnable["id"].asString();
				double chance = spawnable["chance"].asDouble();

				definition.spawnables.emplace_back(std::move(id), (float)chance);
			}
		}

		tileset.tileTypes.insert({ id, std::move(definition) });
	}

	return ce::Result<Tileset>::success(tileset);
}

const char* TilesetLoader::format() const
{
	return ".tileset.json";
}
