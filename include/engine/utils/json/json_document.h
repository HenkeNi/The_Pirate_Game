#pragma once
#include "engine/utils/non_copyable.h"
#include <filesystem>
#include <memory>

#include "json_value.h"

namespace cursed_engine
{
	namespace fs = std::filesystem;

	struct JsonResult
	{
		bool success;
		std::string errorMessage;
	};

	class JsonDocument
	{
	public:
		JsonDocument();
		~JsonDocument();

		JsonResult loadFromFile(const fs::path& path);

		JsonValue root() const;
		//const JsonValue& root() const;
		//JsonValue root();

		JsonValue operator[](const char* key) const;
		//const JsonValue& operator[](const std::string& path) const;
		//JsonValue operator[](const std::string& path);

	private:
		//JsonValue m_root;
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}


/*
* 	class JsonDocument : private NonCopyable
	{
	public:
		JsonDocument();
		~JsonDocument();

		JsonDocument(JsonDocument&&) = delete;
		JsonDocument& operator=(JsonDocument&&) = delete;

		JsonResult111 loadFromFile(const fs::path& path);

		[[nodiscard]] bool isObject(std::string_view key) const;
		[[nodiscard]] bool isArray() const;
		[[nodiscard]] bool isString(const std::string& path) const;
		[[nodiscard]] bool isInt() const;
		[[nodiscard]] bool isBool(const std::string& path) const;
		[[nodiscard]] bool isDouble(const std::string& path) const;
		[[nodiscard]] bool isFloat(const std::string& path) const;
		[[nodiscard]] bool isNumber(const std::string& path) const;


	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

*/