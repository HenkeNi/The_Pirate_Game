#include "engine/config/config_manager.h"
#include "engine/core/logger.h"
#include <engine/utils/json/json_document.h>

namespace cursed_engine
{
	// ==================== Helpers ====================

	bool loadAppInfo(const fs::path& path, AppInfo& outAppInfo)
	{
		JsonDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load app info: " + message);
			return false;
		}

		outAppInfo.name = document["name"].asString();
		outAppInfo.version = document["version"].asString();
		outAppInfo.identifier = document["identifier"].asString();

		return true;
	}

	bool loadWindowConfig(const fs::path& path, WindowConfig& outWindowConfig)
	{
		JsonDocument document;

		const auto [success, message] = document.loadFromFile(path);
		if (!success)
		{
			Logger::logError("Failed to load window config: " + message);
			return false;
		}

		outWindowConfig.iconPath = document["icon_path"].asString();
		outWindowConfig.width = document["width"].asInt();
		outWindowConfig.height = document["height"].asInt();
		outWindowConfig.vsync = document["vsync"].asBool();
		outWindowConfig.fullscreen = document["fullscreen"].asBool();

		return true;
	}

	bool loadInputConfig(const fs::path& path, InputConfig& outInputConfig)
	{
		outInputConfig.keyBindings.insert({ (SDL_Scancode)26, Action::MoveUp });
		outInputConfig.keyBindings.insert({ (SDL_Scancode)4, Action::MoveLeft });
		outInputConfig.keyBindings.insert({ (SDL_Scancode)22, Action::MoveDown });
		outInputConfig.keyBindings.insert({ (SDL_Scancode)7, Action::MoveRight });

		return true;
	}

	bool ConfigManager::loadAllConfigs(const fs::path& path)
	{
		bool success = true;

		success &= loadAppInfo(path / "app.json", m_appInfo);
		success &= loadWindowConfig(path / "window_config.json", m_windowConfig);
		success &= loadInputConfig(path / "input_config.json", m_inputConfig);

		return success;
	}
}