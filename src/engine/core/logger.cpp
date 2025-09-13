#include "engine/pch.h"
#include "engine/core/logger.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	void Logger::logInfo(const std::string& msg)
	{
		SDL_Log(msg.c_str());
	}

	void Logger::logWarning(const std::string& msg, int lvl)
	{
		SDL_LogWarn(lvl, msg.c_str());
	}

	void Logger::logError(const std::string& msg, int lvl)
	{
		SDL_LogError(lvl, msg.c_str());
	}
}