#pragma once

namespace cursed_engine
{
	class Logger
	{
	public:
		static void logInfo(const std::string& msg);

		static void logWarning(const std::string& msg, int lvl = 0);

		static void logError(const std::string& msg, int lvl = 0);
	};
}