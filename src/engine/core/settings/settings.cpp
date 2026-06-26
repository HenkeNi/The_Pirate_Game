#include "engine/core/settings/settings.h"
#include "engine/utils/json/json_document.h"
#include "engine/utils/json/json_value.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	bool loadAppInfo(const JsonDocument& document, AppInfo& outAppInfo)
	{
		outAppInfo.name = document["name"].asString();
		outAppInfo.version = document["version"].asString();
		outAppInfo.identifier = document["identifier"].asString();

		return true;
	}

	bool loadWindowConfig(const JsonValue& jsonValue, WindowConfig& outWindowConfig)
	{
		outWindowConfig.iconPath = jsonValue["icon_path"].asString();
		outWindowConfig.width = jsonValue["width"].asInt();
		outWindowConfig.height = jsonValue["height"].asInt();
		outWindowConfig.vsync = jsonValue["vsync"].asBool();
		outWindowConfig.fullscreen = jsonValue["fullscreen"].asBool();

		return true;
	}

	bool loadInputConfig(const JsonValue& jsonValue, InputConfig& outInputConfig)
	{
		outInputConfig.keyBindings.insert({ Key::W, Action::MoveUp });
		outInputConfig.keyBindings.insert({ Key::A, Action::MoveLeft });
		outInputConfig.keyBindings.insert({ Key::S, Action::MoveDown });
		outInputConfig.keyBindings.insert({ Key::D, Action::MoveRight });

		return true;
	}

	bool loadResourceConfig(const JsonValue& jsonValue, ResourceConfig& resourceConfig)
	{
		for (const auto& supportedFormat : jsonValue["supported_image_formats"].asArray())
		{
			resourceConfig.validTextureFormats.push_back(supportedFormat.asString());
		}

		for (const auto& supportedFormat : jsonValue["supported_audio_formats"].asArray())
		{
			resourceConfig.validAudioFormats.push_back(supportedFormat.asString());
		}

		for (const auto& supportedFormat : jsonValue["supported_font_formats"].asArray())
		{
			resourceConfig.validFontFormats.push_back(supportedFormat.asString());
		}

		return true;
	}

	std::string extractResourceID(const std::filesystem::path& path) noexcept
	{
		return path.stem().string();
		/*std::string filename = path.filename().string();

		for (const auto& extension : extensions)
		{
			if (filename.ends_with(extension))
			{
				return filename.substr(0, filename.length() - extension.length());
			}
		}

		size_t firstDot = filename.find('.');
		if (firstDot != std::string::npos && firstDot > 0)
		{
			return filename.substr(0, firstDot);
		}

		return filename;*/
	}

	bool hasValidFileExtension(const std::string& filename, const std::vector<std::string>& extensions) noexcept
	{
		for (const auto& extension : extensions)
		{
			if (filename.ends_with(extension))
				return true;
		}

		return false;
	}

	bool loadAssetPaths(ResourceConfig& resourceConfig)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();

			std::string filename = path.string();

			if (hasValidFileExtension(filename, resourceConfig.validTextureFormats)
				|| hasValidFileExtension(filename, resourceConfig.validAudioFormats)
				|| hasValidFileExtension(filename, resourceConfig.validFontFormats))
			{
				auto id = extractResourceID(path);
				resourceConfig.resourceIdToPath.insert_or_assign(std::move(id), path.string());
			}
		}

		return true;
	}

	// TODO; PUT IN CONFIG STRUCT? Read from json...?
	// TODO; put in utility file? FileUtils? static member?
	/*static const std::vector<std::string> extensions = {
		".jpeg",
		".png",
		".bmp",
		".mp3",
		".wav",
	};*/

	Settings::Settings(EventBus& eventBus)
		: m_eventBus{ eventBus }
	{
	}

	bool Settings::loadConfig(const std::filesystem::path& path)
	{
		m_config.resource.assetRoot = "../assets/"; // find root instead?

		JsonDocument document;
		JsonResult result = document.loadFromFile(path);

		if (!result.success)
		{
			Logger::logError("Failed to load configs!");
			return false;
		}

		bool success = true;

		success &= loadAppInfo(document, m_config.appInfo);
		success &= loadWindowConfig(document["window"], m_config.window);
		success &= loadInputConfig(document["input"], m_config.input);
		success &= loadResourceConfig(document["resource"], m_config.resource);

		success &= loadAssetPaths(m_config.resource);

		return success;
	}

	void Settings::saveConfig(const std::filesystem::path& path)
	{
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
}