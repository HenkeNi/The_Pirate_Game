#include "engine/utils/json_utils.h"
#include "engine/core/logger.h"
#include <rapidjson/document.h>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cassert>

namespace cursed_engine
{
	//JsonHandler::JsonHandler()
	//	: m_isLoaded{ false }
	//{
	//}

	//JsonHandler::JsonHandler(const fs::path& path)
	//	: m_isLoaded{ false }
	//{
	//	loadFromFile(path);
	//}

	//JsonResult JsonHandler::loadFromFile(const fs::path& path)
	//{
	//	constexpr std::string_view logPrefix = "[JsonParser::loadFromFile] ";

	//	if (!fs::exists(path))
	//	{
	//		const std::string message = "File does not exist: " + path.string();
	//		Logger::logError(std::string(logPrefix) + message); // TODO; dont log here?? make caller do it?

	//		return { false, message };
	//	}

	//	std::ifstream ifs(path, std::ios::binary | std::ios::ate);

	//	if (!ifs)
	//	{
	//		const std::string message = "Unable to open file: " + path.string();
	//		Logger::logError(std::string(logPrefix) + message);

	//		return { false, message };
	//	}

	//	std::streamsize size = ifs.tellg();
	//	ifs.seekg(0, std::ios::beg);

	//	std::string content(static_cast<size_t>(size), '\0');
	//	if (!ifs.read(content.data(), size))
	//	{
	//		const std::string message = "Failed to read contents of file: " + path.string();
	//		Logger::logError(std::string(logPrefix) + message);

	//		return { false, message };
	//	}

	//	rapidjson::Document document;
	//	document.Parse(content.data(), content.size());

	//	if (document.HasParseError())
	//	{
	//		const std::string message = "JSON parse error in file: " + path.string();

	//		Logger::logError(std::string(logPrefix) + message);
	//		return { false, message };
	//	}

	//	m_document.CopyFrom(document, document.GetAllocator());
	//	m_isLoaded = true;

	//	return { true, "Successfully loaded document: " + path.string() };
	//}

	//std::optional<rapidjson::Value::Object> JsonHandler::getObject(const char* key)
	//{
	//	constexpr std::string_view logPrefix = "[JsonParser::getObject] - ";

	//	if (!m_isLoaded)
	//	{
	//		Logger::logError(std::string(logPrefix) + "Document not loaded!");
	//		return std::nullopt;
	//	}

	//	if (!key || m_document.HasMember(key))
	//	{
	//		Logger::logError(std::string(logPrefix) + "Missing key: " + (key ? key : "<null>"));
	//		return std::nullopt;
	//	}

	//	if (!m_document[key].IsObject())
	//	{
	//		Logger::logError(std::string(logPrefix) + "Key is not an object: " + key);
	//		return std::nullopt;
	//	}

	//	auto object = m_document[key].GetObject();
	//}




	PropertyValue parsePropertyValue(const rapidjson::Value& value)
	{
		if (value.IsObject())
		{
			std::unordered_map<std::string, PropertyValue> values;

			for (const auto& val : value.GetObject())
			{
				values.insert({ val.name.GetString(), parsePropertyValue(val.value) });
			}

			return values;
		}
		else if (value.IsArray())
		{
			std::vector<PropertyValue> values;

			for (const auto& val : value.GetArray())
			{
				values.push_back(parsePropertyValue(val));
			}

			return values;
		}
		else if (value.IsInt())
		{
			return value.GetInt();
		}
		else if (value.IsFloat())
		{
			return value.GetFloat();
		}
		else if (value.IsDouble())
		{
			return static_cast<float>(value.GetDouble());
		}
		else if (value.IsString())
		{
			return std::string(value.GetString());
		}
		else if (value.IsBool())
		{
			return value.GetBool();
		}
		else if (value.IsNull())
		{
			return nullptr;
		}

		throw std::runtime_error("Unsupported JSON type");
	}








	JSONDocument::JSONDocument()
		: m_isLoaded{ false }
	{
		//m_document.SetNull();
	}

	JsonResult JSONDocument::loadFromFile(const fs::path& path)
	{
		constexpr std::string_view logPrefix = "[JsonParser::loadFromFile] ";

		if (!fs::exists(path))
		{
			const std::string message = "File does not exist: " + path.string();
			Logger::logError(std::string(logPrefix) + message);

			return { false, message };
		}

		std::ifstream ifs(path, std::ios::binary | std::ios::ate);

		if (!ifs)
		{
			const std::string message = "Unable to open file: " + path.string();
			Logger::logError(std::string(logPrefix) + message);

			return { false, message };
		}

		std::streamsize size = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		std::string content(static_cast<size_t>(size), '\0');
		if (!ifs.read(content.data(), size))
		{
			const std::string message = "Failed to read contents of file: " + path.string();
			Logger::logError(std::string(logPrefix) + message);

			return { false, message };
		}

		rapidjson::Document document;
		document.Parse(content.data(), content.size());

		if (document.HasParseError())
		{
			const std::string message = "JSON parse error in file: " + path.string();

			Logger::logError(std::string(logPrefix) + message);
			return { false, message };
		}

		m_document.CopyFrom(document, m_document.GetAllocator());
		m_isLoaded = true;

		return { true, "Successfully loaded document: " + path.string() };
	}


	
}