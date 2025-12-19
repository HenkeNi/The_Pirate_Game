#include "engine/assets/asset_loaders.h"
#include "engine/utils/json_utils.h"
#include "engine/core/logger.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	TextureAtlasLoader::TextureAtlasLoader(EngineResources& resources)
		: m_resources{ resources }
	{
	}

	std::optional<TextureAtlas> cursed_engine::TextureAtlasLoader::load(const std::string& path) const
	{
		return TextureAtlas();
	}

	SpriteSheetLoader::SpriteSheetLoader(EngineResources& resources)
		: m_resources{ resources }
	{
	}

	std::optional<SpriteSheet> SpriteSheetLoader::load(const std::string& path) const
	{
		JSONDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load SpriteSheet: " + message);
			return std::nullopt;
		}

		SpriteSheet spriteSheet;
		
		spriteSheet.textureID = document["texture"].to<std::string>();
		
		//std::string textureID = document["texture"].to<std::string>(); // rename "texture_path"?
		//auto textureHandle = m_resources.getHandle<Texture>(textureID);

		//if (!textureHandle.isValid())
		//{
		//	Logger::logWarning("[SpriteSheetLoader::Load] - Invalid texture handle!");
		//	return std::nullopt;
		//}
		//spriteSheet.textureHandle = textureHandle;

		int frameWidth = document["frame_width"].to<int>(); // TODO!
		int frameHeight = document["frame_height"].to<int>();

		spriteSheet.rows = document["rows"].to<int>();
		spriteSheet.columns = document["columns"].to<int>();

		document["animations"].forEach([&](std::string name, const rapidjson::Value& value)
			{
				auto n = name;

				std::vector<int> frames;

				for (const auto& frame : value.GetArray())
				{
					int frameIndex = frame.GetInt();
					frames.push_back(frameIndex);
				}



				// Create animation...
			});

			//"frame_width": 117,
			//"frame_height" : 127,
			//"rows" : 6,
			//"columns" : 3,
			//"animations" : {
			//"idle_default": [0, 1, 2] ,
			//	"idle_special" : [3, 4, 5] ,
			//	"walk_right" : [6, 7, 8] ,
			//	"walk_left" : [9, 10, 11] ,
			//	"walk_down" : [12, 13, 14] ,
			//	"walk_up" : [15, 16, 17]
		
	
		return spriteSheet;
	}

	std::optional<Prefab> PrefabLoader::load(const std::string& path) const
	{
		JSONDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load prefab: " + message);
			return std::nullopt;
		}

		Prefab prefab;

		document["components"].forEach([&](std::string name, const rapidjson::Value& value)
			{
				ComponentProperties properties;
				//auto values = parsePropertyValue(value);

				if (value.IsObject())
				{
					// TODO; dont? or maybe do?
					for (const auto& [name, val] : value.GetObject())
					{
						auto property = parsePropertyValue(val);
						properties.insert({ name.GetString(), std::move(property) });
					}
				}

				prefab.components.insert({ name, std::move(properties) });
			});

		prefab.name = document["name"].to<std::string>(); // TODO; use name! store in prefab!
		return prefab;
	}

}