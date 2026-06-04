#include "engine/pch.h"
#include "engine/core/logger.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	void Logger::logInfo(const std::string& msg)
	{
		SDL_Log(msg.c_str());
	}

	void Logger::logInfoExtended(const std::string& msg, const std::source_location& loc)
	{
		std::string message = std::string(loc.function_name()) + " -> " + msg;
		SDL_Log(message.c_str());
	}

	void Logger::logWarning(const std::string& msg, int lvl)
	{
		SDL_LogWarn(lvl, msg.c_str());
	}

	void Logger::logWarningExtended(const std::string& msg, int lvl, const std::source_location& loc)
	{
		//auto func = __PRETTY_FUNCTION__;
		auto func = __FUNCSIG__;
		std::string message = std::string(loc.function_name()) + " -> " + msg;
		SDL_LogWarn(lvl, message.c_str());
	}

	void Logger::logError(const std::string& msg, int lvl)
	{
		SDL_LogError(lvl, msg.c_str());
	} 

	void Logger::logErrorExtended(const std::string& msg, int lvl, const std::source_location& loc)
	{
		std::string message = std::string(loc.function_name()) + " -> " + msg;
		SDL_LogError(lvl, message.c_str());
	}
}