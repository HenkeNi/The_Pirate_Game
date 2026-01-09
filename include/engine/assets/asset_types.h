#pragma once
#include "engine/resources/resource_handle.h"
#include "engine/math/vec2.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

#include "engine/rendering/animation_types.h" // TEMP!

namespace cursed_engine
{
	// using TextureHandle = ResourceHandle<class Texture>;

	struct AtlasRegion
	{
		std::string name;
		IVec2 pivot;
		IVec2 size;
		int x, y, w, h;
	};

	struct SpriteDefinition
	{
		std::string name;
		int rect[4]; // change to a rect struct!
		IVec2 pivot;
		IVec2 size;
	};

	struct TextureAtlas
	{
		std::string textureID;
		//TextureHandle texture;
		std::vector<AtlasRegion> regions;

		// or vector of sprite definitions...? UV's
	};

	
	struct SpriteSheet
	{
		std::string textureID;
		//TextureHandle textureHandle;
		
		// width? height?

		std::vector<SpriteDefinition> definitions;

		int rows = 0;
		int columns = 0;

		//std::vector<Animation> animations;
		std::unordered_map<std::string, Animation> idToAnimations; // or nameToAnimations?

		// TODO; store sequences?

		//std::vector<int> frameIndices; // TODO; store frame here??
		float frameDuration = 0.f;
		bool loop = true;
	};

	// TODO; put in an AssetTypes? together with TextureAtlas, SpriteSheet, etc?

	// put in prefab registry?


	struct PropertyValue : std::variant<
		std::nullptr_t,
		bool,
		int,
		float,
		std::string,
		std::unordered_map<std::string, PropertyValue>,
		std::vector<PropertyValue>>
	{
		using variant::variant;

		template <typename T>
		const T* as() const noexcept
		{
			return std::get_if<T>(this);
		}

		template <typename T>
		T getOr(const T& fallback) const noexcept
		{
			if (auto p = std::get_if<T>(this))
				return *p;

			return fallback;
		}

		bool isNumeric() const noexcept
		{
			return std::holds_alternative<int>(*this) || std::holds_alternative<float>(*this); // TODO; double check
		}
	};



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