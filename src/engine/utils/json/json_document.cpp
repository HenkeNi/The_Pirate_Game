#include "engine/utils/json/json_document.h"
#include "engine/core/result.h"
#include <rapidjson/document.h>
#include <fstream>
#include <format>

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

	JsonDocument::JsonDocument(const fs::path& path)
		: m_impl{ std::make_unique<JsonDocument::Impl>() }
	{
		loadFromFile(path);
	}

	JsonDocument::~JsonDocument() = default;

	Result<void> JsonDocument::loadFromFile(const fs::path& path)
	{
		if (!fs::exists(path))
		{
			return Result<void>::failure(std::format("Not valid file: {}", path.string()));
		}

		std::ifstream ifs(path, std::ios::binary | std::ios::ate);

		if (!ifs)
		{
			return Result<void>::failure(std::format("File could not be opened: {}", path.string()));
		}

		std::streamsize size = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		std::string content(static_cast<size_t>(size), '\0');
		if (!ifs.read(content.data(), size))
		{
			return Result<void>::failure(std::format("File could not be read: {}", path.string()));
		}

		rapidjson::Document document;
		document.Parse(content.data(), content.size());

		if (document.HasParseError())
		{
			return Result<void>::failure(std::format("Document had parse errors. Path {}", path.string())); // TODO: use document.GetParseError()?
		}

		m_impl->document.CopyFrom(document, m_impl->document.GetAllocator());
		return Result<void>::success();
	}

	JsonValue JsonDocument::root() const
	{
		return JsonValue{ &m_impl->document };
	}

	bool JsonDocument::hasParseError() const
	{
		return m_impl->document.HasParseError();
	}

	bool JsonDocument::hasMember(const char* member) const
	{
		return m_impl->document.HasMember(member);
	}

	bool JsonDocument::isLoaded() const noexcept
	{
		return m_impl && !m_impl->document.HasParseError(); // or store own member; bool isLoaded in impl
	}

	JsonValue JsonDocument::operator[](std::string_view key) const
	{
		assert(m_impl->document.HasMember(rapidjson::StringRef(key.data(), key.size())) && "JsonDocument::operator[] - Member not found!");
		return JsonValue{ &m_impl->document[key.data()]};
	}
}