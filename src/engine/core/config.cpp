#include "engine/core/config.h"
#include "engine/core/result.h"
#include "engine/utils/json/json_document.h"
#include "engine/utils/json/json_value.h"
#include <format>
#include <initializer_list>
#include <string>
#include <string_view>

namespace
{
	using namespace cursed_engine;

	constexpr const char* errorMessage = "Failed to load configs.";

	// TODO: static functions in Json? Helpers?
	Result<void> validateMembers(const JsonDocument& document, std::initializer_list<std::string_view> members)
	{
		for (const auto& member : members)
		{
			if (!document.hasMember(member.data()))
			{
				return Result<void>::failure(std::format("Missing required field: \'{}\'", member));
			}
		}

		return Result<void>::success();
	}

	Result<void> validateMembers(const JsonValue& value, std::initializer_list<std::string_view> members)
	{
		for (const auto& member : members)
		{
			if (!value.hasMember(member.data()))
			{
				return Result<void>::failure(std::format("Missing required field: \'{}\'", member));
			}
		}

		return Result<void>::success();
	}

	Result<Config::AppInfo> loadAppInfo(const JsonDocument& document)
	{
		if (Result<void> result = validateMembers(document, { "name", "version", "identifier" }); !result)
		{
			return Result<Config::AppInfo>::failure(std::format("Failed to read \'AppInfo\'. {}", result.message()));
		}

		Config::AppInfo appInfo;
		appInfo.name = document["name"].asString();
		appInfo.version = document["version"].asString();
		appInfo.identifier = document["identifier"].asString();

		return Result<Config::AppInfo>::success(std::move(appInfo));
	}

	Result<Config::Audio> loadAudioConfig(const JsonValue& value)
	{
		if (Result<void> result = validateMembers(value, { "sfx_track_count", "music_track_count", "ambience_track_count", "voice_track_count", "ui_track_count" }); !result)
		{
			return Result<Config::Audio>::failure(std::format("Failed to read \'AudioConfig\'. {}", result.message()));
		}

		Config::Audio audioConfig{};
		audioConfig.sfxTrackCount = value["sfx_track_count"].asInt();
		audioConfig.musicTrackCount = value["music_track_count"].asInt();
		audioConfig.ambienceTrackCount = value["ambience_track_count"].asInt();
		audioConfig.voiceTrackCount = value["voice_track_count"].asInt();
		audioConfig.uiTrackCount = value["ui_track_count"].asInt();

		return Result<Config::Audio>::success(std::move(audioConfig));
	}

	Result<Config::Network> loadNetworkConfig(const JsonValue& value)
	{
		if (Result<void> result = validateMembers(value, { "port", "ip" }); !result)
		{
			return Result<Config::Network>::failure(std::format("Failed to read \'NetworkConfig\'. {}", result.message()));
		}

		Config::Network networkConfig{};
		networkConfig.port = value["port"].asInt();
		networkConfig.ip = value["ip"].asString();

		return Result<Config::Network>::success(std::move(networkConfig));
	}

	Result<Config::Resource> loadResourceConfig(const JsonValue& value)
	{
		if (Result<void> result = validateMembers(value, { "supported_image_formats", "supported_audio_formats", "supported_font_formats" }); !result)
		{
			return Result<Config::Resource>::failure(std::format("Failed to read \'ResourceConfig\'. {}", result.message()));
		}

		Config::Resource resourceConfig{};

		//for (const JsonValue& format : value["supported_image_formats"].asArray())
		//{
		//	resourceConfig.validTextureFormats.push_back(format.asString());
		//}

		//for (const JsonValue& format : value["supported_audio_formats"].asArray())
		//{
		//	resourceConfig.validAudioFormats.push_back(format.asString());
		//}

		//for (const JsonValue& format : value["supported_font_formats"].asArray())
		//{
		//	resourceConfig.validFontFormats.push_back(format.asString());
		//}

		return Result<Config::Resource>::success(std::move(resourceConfig));
	}

	Result<Config::Window> loadWindowConfig(const JsonValue& value)
	{
		if (Result<void> result = validateMembers(value, { "icon_path", "borderless", "always_on_top", "min_size" }); !result)
		{
			return Result<Config::Window>::failure(std::format("Failed to read \'WindowConfig\'. {}", result.message()));
		}

		Config::Window windowConfig{};
		windowConfig.iconPath = value["icon_path"].asString();
		windowConfig.borderless = value["borderless"].asBool();
		windowConfig.alwaysOnTop = value["always_on_top"].asBool();
		windowConfig.minSize.x = value["min_size"]["width"].asInt();
		windowConfig.minSize.y = value["min_size"]["height"].asInt();

		return Result<Config::Window>::success(std::move(windowConfig));
	}
}

namespace cursed_engine
{
	Result<void> Config::load(const std::filesystem::path& path)
	{
		JsonDocument document;
		if (Result<void> result = document.loadFromFile(path); !result)
		{
			return Result<void>::failure(std::format("Failed to load configs. Reason{}", result.message()));
		}

		Result<Config::AppInfo> appInfoResult = loadAppInfo(document);
		if (!appInfoResult)
		{
			return Result<void>::failure(appInfoResult.message());
		}
		
		appInfo = appInfoResult.take();

		Result<Config::Audio> audioResult = loadAudioConfig(document["audio"]);
		if (!audioResult)
		{
			return Result<void>::failure(audioResult.message());
		}

		audio = audioResult.take();

		Result<Config::Resource> resourceResult = loadResourceConfig(document["resource"]);
		if (!resourceResult)
		{
			return Result<void>::failure(resourceResult.message());
		}

		resource = resourceResult.take();

		Result<Config::Window> windowResult = loadWindowConfig(document["window"]);
		if (!windowResult)
		{
			return Result<void>::failure(windowResult.message());
		}

		window = windowResult.take();

		return Result<void>::success();
	}

	std::filesystem::path Config::getDefaultPath() noexcept
	{
		return "../assets/config/engine_config.json";
	}
}