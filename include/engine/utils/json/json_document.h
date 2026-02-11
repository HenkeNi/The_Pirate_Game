#pragma once
#include "engine/utils/non_copyable.h"
#include <filesystem>
#include <memory>

//#include "json_value.h"

namespace cursed_engine
{
	namespace fs = std::filesystem;

	class JsonValue;

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

		[[nodiscard]] JsonValue root() const;

		[[nodiscard]] bool isLoaded() const;

		[[nodiscard]] JsonValue operator[](const char* key) const;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}