#pragma once
#include <engine/audio/audio_controller.h>
#include <engine/utils/containers/registry.hpp>
#include <engine/ecs/component/component_registry.h>
#include <engine/resources/audio/audio_manager.h>
#include <engine/rendering/render_api.h>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

namespace cursed_engine
{
	class EntityFactory;
	//class ComponentRegistry;
	class SystemManager;
	class EventBus;
	class AssetManager;
}

namespace ce = cursed_engine;

struct SceneContext
{
	cursed_engine::EntityFactory* entityFactory;
	cursed_engine::ComponentRegistry* componentRegistry;
	cursed_engine::SystemManager* systemManager;
	cursed_engine::EventBus* eventBus;
	cursed_engine::AssetManager* assetManager;
	cursed_engine::RenderAPI renderAPI;
	cursed_engine::AudioManager* audioManager;
	cursed_engine::AudioController audioController; // wrap this and audio manager in an API? make easier to work with...
};


using SceneId = uint32_t;
using SceneName = std::string;
using SceneRegistry = ce::Registry<SceneName, struct SceneMeta>;
using SceneCreator = std::function<std::unique_ptr<class Scene>(SceneContext)>;

struct SceneMeta
{
	std::filesystem::path path;
	SceneCreator creator; // or SceneLoader
};


enum class SceneTransitionType
{
	Pop,
	Push,
	Swap
};
//
//struct SceneTransition
//{
//	static SceneTransition push(SceneId id) { return { SceneTransitionType::Push, id }; }
//	static SceneTransition pop() { return { SceneTransitionType::Push, 0 }; }
//	static SceneTransition swap(SceneId id) { return { SceneTransitionType::Swap, id }; }
//
//	SceneTransitionType type;
//	SceneId id;
//};


template <DerivedFrom<Scene> T, typename Creator>
void registerScene(SceneRegistry& registry, std::string name, std::filesystem::path path, Creator&& creator)
{
	// use id generator to generate an id?

	SceneMeta meta
	{
		std::move(path),
		std::forward<Creator>(creator)
	};

	registry.emplace(
		std::move(name),
		std::move(meta)
	);
}
//template <DerivedFrom<Scene> T, typename... Args>
//void registerScene(SceneRegistry& registry, std::string name, std::filesystem::path path, Args&&... args)
//{
//	// use id generator to generate an id?
//
//	SceneMeta meta
//	{
//		std::move(path),
//		[args...](SceneContext context) // this requires all args are copieable?
//		{
//			//return std::make_unique<T>(std::move(context), std::forward<Args>(args)...);
//			return std::make_unique<T>(std::move(context), args...); // NOTE: moving here would move args out of capture list??
//		}
//	};
//
//	registry.emplace(
//		std::move(name),
//		std::move(meta)
//	);
//}