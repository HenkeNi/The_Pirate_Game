#pragma once
#include "engine/audio/audio_controller.h"

namespace cursed_engine
{
	class AudioModule
	{
	public:
		bool init();
		void shutdown();

		[[nodiscard]] inline AudioController& getAudioController() noexcept { return m_audioController; }
		[[nodiscard]] inline const AudioController& getAudioController() const noexcept { return m_audioController; }

	private:
		AudioController m_audioController;
	};
}