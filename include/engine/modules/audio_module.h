#pragma once
#include "engine/audio/audio_controller.h"
#include <memory>

namespace cursed_engine
{
	class AudioBackend;
	class AudioCreator;
	enum class Backend;

	class AudioModule
	{
	public:
		AudioModule();
		~AudioModule();

		bool init(Backend backend); // or accept AudioBackend type?
		void shutdown();

		[[nodiscard]] inline AudioController getAudioController() noexcept { return AudioController{ m_backend.get() }; }
		[[nodiscard]] const AudioCreator* getAudioCreator() const noexcept;

	private:
		std::unique_ptr<AudioBackend> m_backend;
	};
}