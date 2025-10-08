#include "engine/config/config_loader.h"
#include "engine/config/config_types.h"
#include "engine/core/logger.h"

namespace cursed_engine
{
	std::optional<EngineConfig> ConfigLoader::loadFromFile(const std::filesystem::path& path)
	{
		// auto doc = JsonUtils::loadDocument();

		if (!std::filesystem::exists(path))
		{
			Logger::logError("[ConfigLoader::LoadFromFile] - Invalid path!");
			return std::nullopt;
		}



	}

	void ConfigLoader::saveToFile(const std::filesystem::path& path)
	{

	}
}