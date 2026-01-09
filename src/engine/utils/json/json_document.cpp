#include "engine/utils/json/json_document.h"
#include "engine/core/logger.h"
#include <rapidjson/document.h>
#include <fstream>


#include "engine/utils/json/json_value.h"

namespace cursed_engine
{
	struct JsonDocument::Impl
	{
		rapidjson::Document document;
	};

	JsonDocument::JsonDocument()
		: m_impl{ std::make_unique<JsonDocument::Impl>() }
	{
	}

	JsonDocument::~JsonDocument() = default;

	JsonResult JsonDocument::loadFromFile(const fs::path& path)
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

		//m_root.m_value = &document;

		//m_document.CopyFrom(document, m_document.GetAllocator());
		m_impl->document.CopyFrom(document, m_impl->document.GetAllocator());
		//m_impl->isLoaded = true;

		return { true, "Successfully loaded document: " + path.string() };
	}

	JsonValue JsonDocument::root() const
	{
		return JsonValue{ &m_impl->document };
	}

	JsonValue JsonDocument::operator[](const char* key) const
	{
		return JsonValue{ &m_impl->document[key]};
	}


	//struct JsonDocument::Impl
	//{
	//	rapidjson::Document document;
	//	bool isLoaded = false;
	//};

	///*class JsonValue
	//{

	//};*/

	//
	//JsonDocument::JsonDocument()
	//	: m_impl{ std::make_unique<JsonDocument::Impl>() }, m_isLoaded{ false }
	//{
	//}

	//JsonDocument::~JsonDocument()
	//{
	//}

	//JsonResult111 JsonDocument::loadFromFile(const fs::path& path)
	//{
	//	constexpr std::string_view logPrefix = "[JsonParser::loadFromFile] ";

	//	if (!fs::exists(path))
	//	{
	//		const std::string message = "File does not exist: " + path.string();
	//		Logger::logError(std::string(logPrefix) + message);

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

	//	m_impl->document.CopyFrom(document, m_impl->document.GetAllocator());
	//	m_impl->isLoaded = true;

	//	return { true, "Successfully loaded document: " + path.string() };
	//}

	//bool JsonDocument::isObject(std::string_view key) const
	//{
	//	return false;
	//}

	//bool JsonDocument::isArray() const
	//{
	//	return false;
	//}

	//bool JsonDocument::isString(const std::string& path) const
	//{
	//	return false;
	//}

	//bool JsonDocument::isInt() const
	//{
	//	return false;
	//}

	//bool JsonDocument::isBool(const std::string& path) const
	//{
	//	return false;
	//}

	//bool JsonDocument::isDouble(const std::string& path) const
	//{
	//	return false;
	//}

	//bool JsonDocument::isFloat(const std::string& path) const
	//{
	//	return false;
	//}

	//bool JsonDocument::isNumber(const std::string& path) const
	//{
	//	return false;
	//}

}