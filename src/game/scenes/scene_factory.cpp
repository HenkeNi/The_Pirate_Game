#include "game/scenes/scene_factory.h"
#include "game/scenes/scene.h"

SceneFactory::SceneFactory(SceneContext context)
	: m_context{ std::move(context) }
{
}

void SceneFactory::init(SceneContext context)
{
	m_context = std::move(context);
}

std::unique_ptr<Scene> SceneFactory::createScene(const std::string& scene) const
{
	if (auto it = m_registry.find(scene); it != m_registry.end())
	{
		return it->second.creator(m_context);
	}

	return nullptr;
}

std::filesystem::path SceneFactory::getFilePath(const std::string& scene) const noexcept
{
	if (auto it = m_registry.find(scene); it != m_registry.end())
	{
		return it->second.path;
	}

	return std::filesystem::path{};
}
