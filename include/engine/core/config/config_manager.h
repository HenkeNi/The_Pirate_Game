#pragma once
#include "config_types.h"
#include "engine/platform/input/input_handler.h"
//#include "engine/platform/input/action.h"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace cursed_engine
{
	namespace fs = std::filesystem;

	struct AppInfo
	{
		std::string name = "";
		std::string identifier = "";
		std::string version = "";
	};

	struct WindowConfig
	{
		std::string iconPath = "";
		std::string title = ""; // here?

		// Dimensions
		int width = 1280;
		int height = 720;

		// Display Settings
		bool fullscreen = false;
		bool vsync = true;
		bool resizable = true;
	};

	// shouldnt know about actions??
	// TODO; forward declare SDL_Scancode?

	struct InputConfig
	{
		using KeyBindings = std::unordered_map<Key, Action>;

		InputConfig() = default;

		InputConfig(const KeyBindings& bindings, float sensitivity)
			: keyBindings{ bindings }, mouseSensitivity{ sensitivity }
		{
		}

		KeyBindings keyBindings;
		float mouseSensitivity = 1.f;
	};

	struct AudioConfig
	{
		float masterVolume = 1.f;
		float musicVolume = 0.8f;
		float sfxVolume = 0.9f;
	};

	struct RenderConfig
	{
		float brightness = 1.f;
		float contrast = 1.f;
	};

	struct ResourceConfig
	{
		std::vector<std::string> validTextureFormats;
		std::vector<std::string> validAudioFormats;
		std::vector<std::string> validFontFormats;
		std::unordered_map<std::string, std::string> resourceIdToPath; // or create ResourceRegistry class?

		uint32_t framesBeforeUnload = 4000;
		float offloadDelaySeconds = 5.0f;
		bool autoOffload = true;
		bool lazyLoad = true;
	};

	// Config manager instead?
	/*struct EngineConfig
	{
		AppInfo appInfo;
		WindowConfig windowConfig;
		InputConfig inputConfig;
		AudioConfig audioConfig;
		RenderConfig renderConfig;
	};*/

	// config loader?

	// Settings? EngineConfig? ConfigHandler? or POD struct EngineConfig and ConfigLoader helper class?
	class ConfigManager
	{
	public:
		bool loadConfig(const fs::path& path); // or config loader?
		
		[[nodiscard]] inline const AppInfo& getAppInfo() const noexcept { return m_appInfo; }

		[[nodiscard]] inline const WindowConfig& getWindowConfig() const noexcept { return m_windowConfig; }

		[[nodiscard]] inline const InputConfig& getInputConfig() const noexcept { return m_inputConfig; }

		[[nodiscard]] inline const ResourceConfig& getResourceConfig() const noexcept { return m_resourceConfig; }

		bool saveWindowConfigToFile(const fs::path& path);

	private:
		// bool loadAssetPaths();
		
		WindowConfig m_windowConfig;
		RenderConfig m_renderConfig;

		InputConfig m_inputConfig;
		AudioConfig m_audioConfig;

		ResourceConfig m_resourceConfig;
		AppInfo m_appInfo;
	};
}