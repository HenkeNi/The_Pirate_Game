#pragma once
#include <filesystem>

namespace cursed_engine
{
	class ConfigLoader
	{
	public:
		static struct WindowConfig LoadWindowConfig(const std::filesystem::path& path);
	};
}