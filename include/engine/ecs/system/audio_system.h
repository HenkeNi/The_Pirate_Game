#pragma once
#include "system.h"
#include "engine/audio/audio.h" 

namespace cursed_engine
{
	class AudioController;

	class AudioSystem : public System
	{
	public:
		AudioSystem(AudioManager& audioManager, AudioController& audioController);

		void update(SystemContext& context) override;

	private:
		AudioManager& m_audioManager;
		AudioController& m_audioController;
	};
}