#include "engine/assets/asset_loaders.h"
#include "engine/utils/json_utils.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	std::optional<TextureAtlas> cursed_engine::TextureAtlasLoader::load(const std::string& path) const
	{
		return TextureAtlas();
	}

	std::optional<SpriteSheet> SpriteSheetLoader::load(const std::string& path) const
	{
		// TODO; fix!
		SpriteSheet spriteSheet;

		return spriteSheet;
	}


	//std::optional<std::pair<std::string, Prefab>> PrefabLoader::load(const std::filesystem::path& path) const
//{
//	JSONDocument document;

//	const auto [success, message] = document.loadFromFile(path);
//	if (!success)
//	{
//		Logger::logError("Failed to load prefab: " + message);
//		return std::nullopt;
//	}

//	Prefab prefab;

//	document["components"].forEach([&](std::string name, const rapidjson::Value& value)
//		{
//			ComponentProperties properties;
//			//auto values = parsePropertyValue(value);

//			if (value.IsObject())
//			{
//				// TODO; dont? or maybe do?
//				for (const auto& [name, val] : value.GetObject())
//				{
//					auto property = parsePropertyValue(val);
//					properties.insert({ name.GetString(), std::move(property) });
//				}
//			}

//			prefab.components.insert({ name, std::move(properties) });
//		});

//	std::string name = document["name"].to<std::string>();
//	return std::optional{ std::pair{ std::move(name), std::move(prefab) } };
//}

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