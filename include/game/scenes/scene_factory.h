#pragma once
#include "game/scenes/scene_types.h"
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class Scene;

class SceneFactory
{
public:
	SceneFactory() = default;
	SceneFactory(SceneContext context);
	
	void init(SceneContext context);

	template <typename T>
	bool registerScene(const std::string& name, const std::filesystem::path& path, T&& creatorFunc);

	[[nodiscard]] std::unique_ptr<Scene> createScene(const std::string& scene) const;

	// here? or put meta data elsewhere?
	std::filesystem::path getFilePath(const std::string& scene) const noexcept;

private:
	struct Meta
	{
		std::filesystem::path path;
		std::function<std::unique_ptr<Scene>(SceneContext)> creator;
	};
	
	std::unordered_map<std::string, Meta> m_registry;
	SceneContext m_context;
};

#pragma region Methods

template <typename T>
bool SceneFactory::registerScene(const std::string& name, const std::filesystem::path& path, T&& creatorFunc)
{
	if (m_registry.contains(name))
		return false;

	auto [it, success] = m_registry.insert({ name, Meta{ path, std::move(creatorFunc) } });
	return success;
}

#pragma endregion