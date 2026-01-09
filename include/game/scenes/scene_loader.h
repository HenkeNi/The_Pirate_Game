#pragma once
#include <filesystem>
#include <memory>

class Scene;

class SceneLoader
{
public:
	// return future bool?
	void loadAssets(Scene& scene, const std::filesystem::path& path) const; // rename function?
};