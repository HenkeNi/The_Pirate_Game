#pragma once
#include "config_types.h"
#include <filesystem>

namespace cursed_engine
{
	namespace fs = std::filesystem;

	// Settings? EngineConfig? ConfigHandler? or POD struct EngineConfig and ConfigLoader helper class?
	class ConfigManager
	{
	public:
		bool loadAllConfigs(const fs::path& path);
		
		bool loadAppInfo(const fs::path& path);

		bool loadWindowConfig(const fs::path& path);

		bool loadInputConfig(const fs::path& path);

		bool loadRenderConfig(const fs::path& path);

		bool loadAudioConfig(const fs::path& path);



		bool saveWindowConfigToFile(const fs::path& path);



		[[nodiscard]] inline const AppInfo& getAppInfo() const { return m_appInfo; }

		[[nodiscard]] inline const WindowConfig& getWindowConfig() const { return m_windowConfig; }

		[[nodiscard]] inline const InputConfig& getInputConfig() const { return m_inputConfig; }

	private:
		WindowConfig m_windowConfig;
		RenderConfig m_renderConfig;

		InputConfig m_inputConfig;
		AudioConfig m_audioConfig;

		AppInfo m_appInfo;
	};












}