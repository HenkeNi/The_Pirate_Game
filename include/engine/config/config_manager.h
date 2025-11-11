#pragma once
#include "engine/core/subsystem.h"
#include "config_types.h"
#include <filesystem>

namespace cursed_engine
{
	namespace fs = std::filesystem;

	// Settings? EngineConfig? ConfigHandler? or POD struct EngineConfig and ConfigLoader helper class?
	class ConfigManager : public Subsystem
	{
	public:
		bool loadAllConfigs(const fs::path& path);
		
		[[nodiscard]] inline const AppInfo& getAppInfo() const noexcept { return m_appInfo; }

		[[nodiscard]] inline const WindowConfig& getWindowConfig() const noexcept { return m_windowConfig; }

		[[nodiscard]] inline const InputConfig& getInputConfig() const noexcept { return m_inputConfig; }

		bool saveWindowConfigToFile(const fs::path& path);


	private:
		WindowConfig m_windowConfig;
		RenderConfig m_renderConfig;

		InputConfig m_inputConfig;
		AudioConfig m_audioConfig;

		AppInfo m_appInfo;
	};

}