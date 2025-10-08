#include "engine/utils/json_utils.h"
#include "engine/core/logger.h"
#include <rapidjson/document.h>
#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace cursed_engine
{
	struct JsonLoader::Impl
	{
		rapidjson::Document document;
		std::unordered_map<std::string, rapidjson::Value> property;
		bool isLoaded = false;
	};

	JsonLoader::JsonLoader()
		: m_impl{ std::make_unique<JsonLoader::Impl>() }
	{
	}
	
	JsonLoader::~JsonLoader()
	{
	}

	bool JsonLoader::loadFromFile(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			Logger::logError("[JsonUtils::loadJsonDocument] - Invalid path: " + path.string());
			return false;
		}

		std::ifstream ifs{ path };

		if (!ifs.is_open())
		{
			Logger::logError("[JsonUtils::loadJsonDocument] - Failed to open file: " + path.string());
			return false;
		}

		std::string content{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };

		rapidjson::Document document;
		document.Parse(content.data(), content.size());

		if (document.HasParseError())
		{
			Logger::logError("[JsonUtils::loadJsonDocument] - Failed to parse document. Path: " + path.string());
			return false;
		}

		m_impl->document = std::move(document);
		m_impl->isLoaded = true;
	}

	//JsonLoader& JsonLoader::getObject(std::string_view key)
	//{
	//	 // currentvaleu?

	//	// TODO: insert return statement here
	//}

	int JsonLoader::getInteger(std::string key, std::string parent)
	{
		return 0;
	}









	rapidjson::Document loadDocument(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			Logger::logError("[JsonUtils::loadJsonDocument] - Invalid path: " + path.string());
			throw std::runtime_error("Invalid path: " + path.string());
		}

		std::ifstream ifs{ path };

		if (!ifs.is_open())
		{
			Logger::logError("[JsonUtils::loadJsonDocument] - Failed to open file: " + path.string());
			throw std::runtime_error("Failed to open file: " + path.string());
		}

		std::string content{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };

		rapidjson::Document document;
		document.Parse(content.data(), content.size());

		if (document.HasParseError())
		{
			Logger::logError("[JsonUtils::loadJsonDocument] - Failed to parse document. Path: " + path.string());
			throw std::runtime_error("JSON parse error in file: " + path.string());
		}

		return document;
	}




}