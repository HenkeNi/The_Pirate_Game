#include "engine/assets/asset_loaders.h"
#include "engine/utils/json/json_document.h"
#include "engine/core/logger.h"
#include "engine/resources/resource_manager.hpp"

namespace cursed_engine
{
	PropertyValue parsePropertyValue(const JsonValue& value); // In this file?

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
		JsonDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load SpriteSheet: " + message);
			return std::nullopt;
		}

		SpriteSheet spriteSheet;
		
		spriteSheet.textureID = document["texture"].asString();
		
		//std::string textureID = document["texture"].to<std::string>(); // rename "texture_path"?
		//auto textureHandle = m_resources.getHandle<Texture>(textureID);

		//if (!textureHandle.isValid())
		//{
		//	Logger::logWarning("[SpriteSheetLoader::Load] - Invalid texture handle!");
		//	return std::nullopt;
		//}
		//spriteSheet.textureHandle = textureHandle;

		int frameWidth = document["frame_width"].asInt(); // TODO!
		int frameHeight = document["frame_height"].asInt();

		spriteSheet.rows = document["rows"].asInt();
		spriteSheet.columns = document["columns"].asInt();

		document["animations"].forEachObject([&](const char* name, JsonValue value)
			{
				auto n = name;

				std::vector<int> frames;

				// TODO; fix optional name!
				value.forEachArray([&](JsonValue value) 
					{
						int frameIndex = value.asInt();
						frames.push_back(frameIndex);
					});

				//for (const auto& frame : value.GetArray())
				//{
				//	int frameIndex = frame.GetInt();
				//	frames.push_back(frameIndex);
				//}

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
		JsonDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load prefab: " + message);
			return std::nullopt;
		}

		Prefab prefab;

		document["components"].forEachObject([&](const char* name, JsonValue value)
			{
				ComponentProperties properties;
				//auto values = parsePropertyValue(value);

				if (value.isObject())
				{
					// TODO; dont? or maybe do?

					value.forEachObject([&](const char* name, JsonValue value) 
						{
							auto property = parsePropertyValue(value);
							properties.insert({ name, std::move(property) });
						});

					//for (const auto& [name, val] : value.getObject())
					//{
					//	auto property = parsePropertyValue(val);
					//	properties.insert({ name.GetString(), std::move(property) });
					//}
				}

				prefab.components.insert({ name, std::move(properties) });
			});

		prefab.name = document["name"].asString(); // TODO; use name! store in prefab!
		return prefab;
	}



	PropertyValue parsePropertyValue(const JsonValue& value) // HERE????
	{
		if (value.isObject())
		{
			std::unordered_map<std::string, PropertyValue> values;

			value.forEachObject([&](const char* name, JsonValue value)
				{
					values.insert({ name, parsePropertyValue(value) });
				});

			//for (const auto& val : value.GetObject())
			//{
			//	values.insert({ val.name.GetString(), parsePropertyValue(val.value) });
			//}

			return values;
		}
		else if (value.isArray())
		{
			std::vector<PropertyValue> values;

			value.forEachArray([&](JsonValue value)
				{
					values.push_back(parsePropertyValue(value)); // Move? maybe uneccessary here=
				});
			//for (const auto& val : value.GetArray())
			//{
			//	values.push_back(parsePropertyValue(val));
			//}

			return values;
		}
		else if (value.isInt())
		{
			return value.asInt();
		}
		//else if (value.isFloat())
		//{
		//	return value.GetFloat();
		//}
		else if (value.isDouble())
		{
			return static_cast<float>(value.asDouble()); // ??
		}
		else if (value.isString())
		{
			return std::string(value.asString());
		}
		else if (value.isBool())
		{
			return value.asBool();
		}
		else if (value.isNull())
		{
			return nullptr;
		}

		throw std::runtime_error("Unsupported JSON type");
	}


}