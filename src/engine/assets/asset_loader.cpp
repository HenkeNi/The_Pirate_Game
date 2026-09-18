#include "engine/assets/asset_loader.h"
#include "engine/utils/json/json_document.h"
#include "engine/utils/json/json_value.h"
#include "engine/core/result.h"

namespace cursed_engine
{
	PropertyValue parsePropertyValue(const JsonValue& value); // In this file?

	Result<TextureAtlas> cursed_engine::TextureAtlasLoader::load(const std::filesystem::path& path) const
	{
		JsonDocument document;

		const Result<void> result = document.loadFromFile(path);		
		if (!result.ok())
		{
			return Result<TextureAtlas>::failure(std::format("Failed to load SpriteSheet: ", result.message()));
		}

		TextureAtlas textureAtlas;
		textureAtlas.textureId = document["texture_id"].asString();

		if (document.hasMember("regions"))
		{
			for (const auto& region : document["regions"].asArray())
			{
				int x = region["rect"]["x"].asInt();
				int y = region["rect"]["y"].asInt();

				int w = region["rect"]["w"].asInt();
				int h = region["rect"]["h"].asInt();

				FVec2 pivot{};

				if (region.hasMember("pivot"))
				{
					pivot.x = region["pivot"]["x"].asFloat();
					pivot.y = region["pivot"]["y"].asFloat();
				}
				else
				{
					int x = 20;
				}

				textureAtlas.regions.emplace_back(IRect{ x, y, w, h, }, pivot);
				textureAtlas.idToRegionIndex.insert({ region["identifier"].asString(), textureAtlas.regions.size() - 1 });
			}
		}

		return Result<TextureAtlas>::success(textureAtlas);
	}

	const char* TextureAtlasLoader::format() const
	{
		return ".texture_atlas.json";
	}

	Result<AnimationSet> AnimationLoader::load(const std::filesystem::path& path) const
	{
		JsonDocument document;

		const Result<void> result = document.loadFromFile(path);

		if (!result.ok())
		{
			return Result<AnimationSet>::failure(std::format("Failed to load SpriteSheet: ", result.message()));
		}

		// TODO; pass in texture manager? to get texture handle...

		AnimationSet animationSet;
		animationSet.textureId = document["texture"].asString();

		for (const auto& animation : document["animations"].asArray())
		{
			std::string name = animation["name"].asString();
			std::vector<Animation::Frame> frames;

			for (const auto& frame : animation["frames"].asArray())
			{
				frames.emplace_back(
					frame["region"].asString(),
					frame["duration"].asDouble()
				);
			}

			bool isLooping = animation["looping"].asBool();

			animationSet.animations.insert({ std::move(name), Animation{ std::move(frames), isLooping } });
		}

		return Result<AnimationSet>::success(animationSet);
	}

	const char* AnimationLoader::format() const
	{
		return ".animation.json";
	}

	Result<Prefab> PrefabLoader::load(const std::filesystem::path& path) const
	{
		JsonDocument document;

		const Result<void> result = document.loadFromFile(path);
		if (!result.ok())
		{
			return Result<Prefab>::failure(std::format("Failed to load prefab: ", result.message()));
		}

		Prefab prefab;

		document["components"].forEachProperty([&](const char* name, JsonValue value)
			{
				if (!value.isObject())
				{
					// LogError?
					return;
				}

				ComponentProperties properties;
				//auto values = parsePropertyValue(value);

				// TODO; dont? or maybe do?

				value.forEachProperty([&](const char* name, JsonValue jsonValue) // rename value?
					{
						auto n = name;

						auto property = parsePropertyValue(jsonValue);
						properties.insert({ name, std::move(property) });
					});

				//for (const auto& [name, val] : value.getObject())
				//{
				//	auto property = parsePropertyValue(val);
				//	properties.insert({ name.GetString(), std::move(property) });
				//}


				prefab.components.insert({ name, std::move(properties) });
			});

		prefab.name = document["name"].asString(); // TODO; use name! store in prefab!
		return Result<Prefab>::success(prefab);
	}

	const char* PrefabLoader::format() const
	{
		return ".prefab.json";
	}



	PropertyValue parsePropertyValue(const JsonValue& value) // HERE????
	{
		if (value.isObject())
		{
			std::unordered_map<std::string, PropertyValue> values;

			value.forEachProperty([&](const char* name, JsonValue value)
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

			for (const auto& v : value.asArray())
			{
				values.push_back(parsePropertyValue(v)); // Move? maybe uneccessary here=
			}

			//value.forEachArray([&](JsonValue value)
			//	{
			//		values.push_back(parsePropertyValue(value)); // Move? maybe uneccessary here=
			//	});
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