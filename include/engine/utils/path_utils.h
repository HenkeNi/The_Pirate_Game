#pragma once
#include <filesystem>
#include <string>
// TODO; rename file utils?

namespace cursed_engine
{
	[[nodiscard]] std::string extractAssetID(const std::filesystem::path& path);
	[[nodiscard]] std::string extractResourceID(const std::filesystem::path& path);
}