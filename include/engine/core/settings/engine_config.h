#pragma once
#include "engine/platform/input/input_handler.h"
#include "engine/platform/input/action.h"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

//#include "engine/platform/input/action.h"

namespace cursed_engine
{
	struct WindowConfig
	{
		std::string iconPath = "";
		std::string title = ""; // here? or in app data?

		// Dimensions
		int width = 1280;
		int height = 720;

		// Display Settings
		bool fullscreen = false;
		bool vsync = true;
		bool resizable = true;
	};

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

		std::filesystem::path assetRoot;
		uint32_t framesBeforeUnload = 4000;
		float offloadDelaySeconds = 5.0f;
		bool autoOffload = true;
		bool lazyLoad = true;
	};

	// client side instead?
	struct AppInfo
	{
		std::string name = "";
		std::string identifier = "";
		std::string version = "";
	};

	struct EngineConfig
	{
		WindowConfig window;
		InputConfig input;
		AudioConfig audio;
		RenderConfig render;
		ResourceConfig resource;
		AppInfo appInfo;
	};
}