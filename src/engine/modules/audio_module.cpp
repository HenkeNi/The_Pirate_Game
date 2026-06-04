#include "engine/modules/audio_module.h"
#include <engine/core/logger.h>

namespace cursed_engine
{
	bool AudioModule::init()
	{
		if (!m_audioController.init())
		{
			Logger::logInfo("Failed to initialize AudioController!");
			return false;
		}
		
		Logger::logInfo("-> AudioModule: Success");
		return true;
	}

	void AudioModule::shutdown()
	{
		m_audioController.shutdown();
	}
}