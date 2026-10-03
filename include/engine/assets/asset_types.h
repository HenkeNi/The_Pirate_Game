#pragma once
#include "engine/resources/resource_handle.h"
#include "engine/math/vec2.hpp"
#include "engine/math/rect.hpp"

//#include "engine/assets/asset_manager.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <cassert>

#include "engine/rendering/animation_types.h" // TEMP!

namespace cursed_engine
{
	// using TextureHandle = ResourceHandle<class Texture>;

	struct AtlasRegion
	{
		// consider pixelRect and uvRect (cached uv calculation)
		IRect rect;
		//IVec2 pivot; // or in sprite definition?
		FVec2 pivot{ 0.f, 0.f }; // normalized: (0,0)=top-left, (1,1)=bottom-right, (0.5,0.5)=center


		// Good idea?
		[[nodiscard]] FVec2 getSize() const noexcept { return FVec2{ rect.w, rect.h }; }
	};

	struct TextureAtlas
	{
		std::string textureId; // replace with TextureHandle texture;
		std::vector<AtlasRegion> regions;

		std::unordered_map<std::string, std::size_t> idToRegionIndex;

		[[nodiscard]] inline const AtlasRegion& resolveRegion(const std::string& id) const
		{
			std::size_t index = idToRegionIndex.at(id);
			return regions.at(index);
		}

		//IVec2 textureSize; // set?
	};

	//struct Grid ?

	//struct SpriteDefinition
	//{
	//	std::string name;
	//	int rect[4]; // change to a rect struct!
	//	IVec2 pivot;
	//	IVec2 size;
	//};

	struct Animation
	{
		struct Frame
		{
			std::string regionId;
			//AtlasRegion region;
			float duration;
		};

		std::vector<Frame> frames;
		bool looping;
	};

	struct AnimationSet
	{
		// todo; use int (id) later
		std::unordered_map<std::string, Animation> animations;
		//AssetHandle atlasHandle;
		std::string textureId; // either here or in Animation to allow multiple textures
	};


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
			assert(isType<T>() && "Variant doesn't hold requested type!");
			return std::get_if<T>(this);
		}

		template <typename T>
		T getOr(const T& fallback) const noexcept
		{
			assert(isType<T>() && "Variant doesn't hold requested type!");

			if (auto p = std::get_if<T>(this))
				return *p;

			return fallback;
		}

		[[nodiscard]] bool isNumeric() const noexcept
		{
			return std::holds_alternative<int>(*this) || std::holds_alternative<float>(*this); // TODO; double check
		}

		template <typename T>
		[[nodiscard]] bool isType() const noexcept
		{
			return std::holds_alternative<T>(*this); // constexpr??
		}
	};

	using ComponentProperties = std::unordered_map<std::string, PropertyValue>;

	//class ComponentProperties
	//{
	//public:
		
	//private:
	//};


	// todo, remake? structure like json document?
	struct Prefab
	{
		std::unordered_map<std::string, ComponentProperties> components;
		std::string name;
		// std::vector<std::function<void(Entity)>> componentBuilders;
	};


	//class Prebab
	//{
	//public:
	//	Prebab operator[](size_t index) noexcept;

	//private:
	//	using ComponentId = std::string; // or use type id??
	//	using PropertyKey = std::string;

	//	using ComponentProperties = std::unordered_map<PropertyKey, PropertyValue>;


	//	std::unordered_map<ComponentId, ComponentProperties> components; // component properties or comp.. data?

	//	//template  <typename T>
	//	//const ComponentProperties& get

	//};

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