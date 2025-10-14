#include "engine/config/config_manager.h"
#include "engine/core/logger.h"
#include <engine/utils/json_utils.h>

namespace cursed_engine
{
	bool ConfigManager::loadAllConfigs(const fs::path& path)
	{
		loadAppInfo(path / "app.json");
		loadWindowConfig(path / "window_config.json");

		return true;
	}

	bool ConfigManager::loadAppInfo(const fs::path& path)
	{
		JSONDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load app info: " + message);
			return false;
		}

		m_appInfo.name = document["name"].to<std::string>();
		m_appInfo.version = document["version"].to<std::string>();
		m_appInfo.identifier = document["identifier"].to<std::string>();

		return true;
	}

	bool ConfigManager::loadWindowConfig(const fs::path& path)
	{
		JSONDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load window config: " + message);
			return false;
		}

		m_windowConfig.iconPath = document["icon_path"].to<std::string>();
		m_windowConfig.width = document["width"].to<int>();
		m_windowConfig.height = document["height"].to<int>();
		m_windowConfig.vsync = document["vsync"].to<bool>();
		m_windowConfig.fullscreen = document["fullscreen"].to<bool>();

		return true;
	}

	bool ConfigManager::loadInputConfig(const fs::path& path)
	{
		m_inputConfig.keyBindings.insert({ (SDL_Scancode)26, Action::MoveUp });
		m_inputConfig.keyBindings.insert({ (SDL_Scancode)4, Action::MoveLeft });
		m_inputConfig.keyBindings.insert({ (SDL_Scancode)22, Action::MoveDown });
		m_inputConfig.keyBindings.insert({ (SDL_Scancode)7, Action::MoveRight });

		return true;
	}
}