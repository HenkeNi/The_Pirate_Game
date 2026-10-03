#include "engine/modules/audio_module.h"
#include "engine/audio/audio_controller.h"
#include "engine/core/logger.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/result.h"
#include <format>

namespace cursed_engine
{
	AudioModule::AudioModule()
		: m_audioController{ nullptr }
	{
	}

	AudioModule::~AudioModule()
	{
	}

	bool AudioModule::init(Backend backend)
	{
		Logger::logInfo(std::format("{}[AudioModule] - Initialization started...", log_format::INDENT));

		switch (backend)
		{
		case Backend::SDL:
			m_audioController = std::make_unique<SDLAudioController>();
			Logger::logInfo(std::format("{}[AudioModule] - Selected backend: SDL", log_format::INDENT));
			break;

		default:
			Logger::logInfo(std::format("{}[AudioModule] - Unsupported backend {}", log_format::INDENT, (int)backend));
			return false;
		}

		const Result<void> result = m_audioController->init();

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
		m_audioController->shutdown();
	}

	const AudioCreator* AudioModule::getAudioCreator() const noexcept
	{
		return m_audioController->getAudioCreator();
	}
}