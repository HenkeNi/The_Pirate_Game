#include "engine/modules/audio_module.h"
#include <engine/core/logger.h>
#include <engine/core/result.h>
#include <format>

namespace cursed_engine
{
	bool AudioModule::init()
	{
		Logger::logInfo(std::format("{}[AudioModule] - Initialization started...", log_format::INDENT));

		const Result<void> result = m_audioController.init();

		if (!result.ok())
		{
			Logger::logInfo(std::format("{}[AudioModule] - Initialization failed! Reason: {}", log_format::INDENT, result.message()));
			return false;
		}

		Logger::logInfo(std::format("{}[AudioModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void AudioModule::shutdown()
	{
		m_audioController.shutdown();
	}
}