#pragma once
#include <filesystem>
#include <memory>
#//include <engine/ecs/component/component_registry.h>

namespace cursed_engine
{
	struct ComponentInitContext; // why not working?
}

class Scene;

class SceneLoader
{
public:
	// return future bool?
	void loadAssets(Scene& scene, const std::filesystem::path& path, const cursed_engine::ComponentInitContext& ctx) const; // rename function?
};