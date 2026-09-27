#pragma once
#include <filesystem>
#include <memory>
#include <string_view>

namespace cursed_engine
{
	namespace fs = std::filesystem;

	class JsonValue;

	template <typename T>
	class Result;

	// TODO; delete copy constructor?

	class JsonDocument
	{
	public:
		JsonDocument();
		JsonDocument(const fs::path& path); // make sure works!! or dont? no way of knowing if succesful or not,..
		~JsonDocument();

		Result<void> loadFromFile(const fs::path& path);

		[[nodiscard]] JsonValue root() const;

		[[nodiscard]] bool hasMember(const char* member) const;
		
		[[nodiscard]] bool hasParseError() const;

		[[nodiscard]] bool isLoaded() const noexcept;

		[[nodiscard]] JsonValue operator[](std::string_view key) const; // overload with non const?

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}