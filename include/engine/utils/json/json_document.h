#pragma once
#include "engine/utils/non_copyable.h"
#include <filesystem>
#include <memory>

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
		JsonDocument(const fs::path& path); // make sure works!!
		~JsonDocument();

		JsonResult loadFromFile(const fs::path& path);

		[[nodiscard]] JsonValue root() const;

		[[nodiscard]] bool isLoaded() const noexcept;

		[[nodiscard]] JsonValue operator[](const char* key) const; // overload with non const?

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}