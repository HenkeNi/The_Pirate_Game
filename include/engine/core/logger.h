#pragma once
#include <source_location>

namespace cursed_engine
{
	class Logger
	{
	public:
		static void logInfo(const std::string& msg);

		static void logInfoExtended(const std::string& msg, const std::source_location& loc = std::source_location::current());

		static void logWarning(const std::string& msg, int lvl = 0);

		static void logWarningExtended(const std::string& msg, int lvl = 0, const std::source_location& loc = std::source_location::current());

		static void logError(const std::string& msg, int lvl = 0);

		static void logErrorExtended(const std::string& msg, int lvl = 0, const std::source_location& loc = std::source_location::current());
	};
}