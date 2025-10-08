#pragma once
#include <filesystem>

namespace cursed_engine
{
	struct EngineConfig;

	class ConfigLoader
	{
	public:
		static std::optional<EngineConfig> loadFromFile(const std::filesystem::path& path); // nodiscard?
		static void saveToFile(const std::filesystem::path& path);
	};
}