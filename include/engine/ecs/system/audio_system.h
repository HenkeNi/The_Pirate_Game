#pragma once
#include "system.h"
#include "engine/audio/audio_controller.h"
#include "engine/resources/resources.h"

namespace cursed_engine
{
	class AudioController;
	class EventBus;

	class AudioSystem : public UpdateSystem
	{
	public:
		AudioSystem(AudioController audioController, Resources resources, EventBus* eventBus);

		void update(SystemUpdateContext& context) override;

	private:
		AudioController m_audioController;
		Resources m_resources;
		EventBus* m_eventBus;
	};
}