#include "engine/ecs/system/audio_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/events/event_bus.h"
#include "engine/events/events.h"
#include "engine/audio/audio_controller.h"

namespace cursed_engine
{
	AudioSystem::AudioSystem(AudioManager& audioManager, AudioController& audioController, EventBus& eventBus)
		: m_audioManager{ audioManager }, m_audioController{ audioController }, m_eventBus{ eventBus }
	{
		m_eventBus.subscribe<PlaySoundEvent>(
			[&](PlaySoundEvent e) 
			{
				// or put in queue?
				auto audioHandle = m_audioManager.getHandleById("623175__aphom000__button-click-selection");
				if (const auto* audio = m_audioManager.get(audioHandle))
				{
					m_audioController.playSound(audio->m_stream, audio->m_buffer, audio->m_length);
				}

				int x = 20;
			});
		/*m_eventBus.subscribe<PlaySoundEvent>(
			[](Entity e) 
			{
				int x = 20;
			});*/
	}

	void AudioSystem::update(SystemContext& context)
	{
		// holds map of which sound id to sound?

		auto audioHandle = m_audioManager.getHandleById("707884__dave4884__pirates-song");

		if (audioHandle.isValid())
		{
			int x = 20;
		}

		// How will this work when multiple components referes to the same audio?
		if (const auto* audio = m_audioManager.get(audioHandle))
		{
			m_audioController.playSound(audio->m_stream, audio->m_buffer, audio->m_length);
		}

		////auto audioHandle = resourceManager.getHandle<Audio>("707884__dave4884__pirates-song.wav");
		//const auto& audio = resourceManager.get<Audio>(audioHandle);

		////auto audioHandle2 = resourceManager.getHandle<Audio>("249813__spookymodem__goblin-death.wav");
		//auto audioHandle2 = resourceManager.getHandle<Audio>("249813__spookymodem__goblin-death");
		//const auto& audio2 = resourceManager.get<Audio>(audioHandle2);


		auto view = context.registry.view<AudioComponent>();

		view.forEach([](AudioComponent& audioComponent) 
			{
						

			});
	}
}