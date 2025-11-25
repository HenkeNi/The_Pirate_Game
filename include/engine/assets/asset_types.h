#pragma once
#include "engine/resources/resource_handle.h"
#include "engine/utils/json_utils.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace cursed_engine
{
	struct AtlasRegion
	{
		int x, y, w, h;
	};

	struct TextureAtlas
	{
		TextureHandle texture;
		std::vector<AtlasRegion> regions; // or use Rect?
	};

	struct SpriteSheet
	{
		TextureAtlas* atlas; // or a TextureAtlasAssetHandle?
		//int atlasID;
		std::vector<int> frameIndices;
		float frameDuration;
		bool loop = true;
	};

	// TODO; put in an AssetTypes? together with TextureAtlas, SpriteSheet, etc?

	// put in prefab registry?
	using ComponentProperties = std::unordered_map<std::string, PropertyValue>;

	struct Prefab
	{
		std::unordered_map<std::string, ComponentProperties> components;
		std::string name;
		// std::vector<std::function<void(Entity)>> componentBuilders;
	};

	//namespace prefab
	//{
	//	template <typename T>
	//	T getPropertyOrDefault(const Prefab& prefab, const std::string& key, const T& fallback)
	//	{
	//		if (auto it = prefab.components.find(key); it != prefab.components.end())
	//		{
	//			auto s = it->second;
	//			fix this!
	//			//return *it->second.as<T>();

	//			//if (auto* val = std::get_if<T>(&it->second))
	//			//{
	//			//	return *val;
	//			//}
	//		}

	//		return fallback;
	//	}
	//}

}