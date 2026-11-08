#include "engine/core/settings/settings.h"
#include "engine/utils/json/json_document.h"
#include "engine/utils/json/json_value.h"
#include "engine/platform/input_handler.h" // gets it already from engine config.h...
#include "engine/core/events/event_bus.h"
#include "engine/core/logger.h"
#include "engine/core/result.h"

namespace cursed_engine
{	
	bool loadAssetSettings(const JsonValue& value, AssetSettings& outAssetSettings)
	{
		return true;
	}

	bool loadAudioSettings(const JsonValue& value, AudioSettings& outAudioSettings)
	{
		const JsonValue& volumes = value["volumes"];
		outAudioSettings.musicVolume = volumes["music"].asFloat();
		outAudioSettings.sfxVolume = volumes["sfx"].asFloat();
		outAudioSettings.ambienceVolume = volumes["ambience"].asFloat();
		outAudioSettings.voiceVolume = volumes["voice"].asFloat();
		outAudioSettings.uiVolume = volumes["ui"].asFloat();
		
		outAudioSettings.masterVolume = volumes["master"].asFloat();

		return true;
	}

	bool loadGraphicsSettings(const JsonValue& value, GraphicsSettings& outGraphicsSettings)
	{
		outGraphicsSettings.vsync = value["vsync"].asBool();

		return true;

	}

	bool loadInputSettings(const JsonValue& value, InputSettings& outInputSettings)
	{

		return true;
	}
	bool loadLocalizationSettings(const JsonValue& value, LocalizationSettings& outLocalizationSettings)
	{ 
		outLocalizationSettings.language = value["language"].asString();
		return true;
	}

	bool loadWindowSettings(const JsonValue& value, WindowSettings& outWindowSettings)
	{
		outWindowSettings.size.x = value["width"].asInt();
		outWindowSettings.size.y = value["height"].asInt();
		// size, etc...

		return true;
	}

	//bool loadAssetPaths(ResourceConfig& resourceConfig)
	//{
	//	for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets"))
	//	{
	//		if (!entry.is_regular_file())
	//			continue;

	//		const auto& path = entry.path();

	//		std::string filename = path.string();

	//		if (hasValidFileExtension(filename, resourceConfig.validTextureFormats)
	//			|| hasValidFileExtension(filename, resourceConfig.validAudioFormats)
	//			|| hasValidFileExtension(filename, resourceConfig.validFontFormats))
	//		{
	//			auto id = extractResourceId(path);
	//			resourceConfig.resourceIdToPath.insert_or_assign(std::move(id), path.string());
	//		}
	//	}

	//	return true;
	//}

	Settings::Settings(EventBus& eventBus)
		: m_eventBus{ eventBus }
	{
	}

	Result<void> Settings::save(const std::filesystem::path& path)
	{
		return Result<void>::success();
	}

	Result<void> Settings::load(const std::filesystem::path& path)
	{
		JsonDocument document;

		if (Result<void> result = document.loadFromFile(path); !result)
		{
			Logger::logError(result.message());
			return result;
		}

		bool success = true;

		success &= loadAssetSettings(document["assets"], m_assets);
		success &= loadAudioSettings(document["audio"], m_audio);
		success &= loadGraphicsSettings(document["graphics"], m_graphics);
		success &= loadInputSettings(document["input"], m_input);
		success &= loadLocalizationSettings(document["localization"], m_localization);
		success &= loadWindowSettings(document["window"], m_window);

		return success ?
			Result<void>::success() : Result<void>::failure("Failed to load configs");
	}

	void Settings::applySettings()
	{
		/*LOG_INFO(
			"Applying window settings: {}x{}, fullscreen={}, vsync={}",
			width,
			height,
			fullscreen,
			vsync);*/
	}

	//std::filesystem::path Settings::getConfigPath()
	//{
	//	static const std::filesystem::path path = [] {

	//		return "../assets/config/engine_config.json";

	//		//auto exePath = getExecutableDirectory(); // custom function
	//		//return exePath / "assets" / "config" / "engine_config.json";
	//		}();
	//	return path;
	//}
	std::filesystem::path Settings::getDefaultPath() noexcept
	{
		//static const std::filesystem::path path = [] { return "../assets/"; }();
		return "../assets/settings/settings.json";
	}
}