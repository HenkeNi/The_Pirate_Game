#pragma once
#include <memory>

namespace cursed_engine
{
	class AudioController;
	class AudioCreator;
	enum class Backend;

	class AudioModule
	{
	public:
		AudioModule();
		~AudioModule();

		bool init(Backend backend); // or accept AudioBackend?
		void shutdown();

		[[nodiscard]] inline AudioController& getAudioController() noexcept { return *m_audioController; }
		[[nodiscard]] inline const AudioController& getAudioController() const noexcept { return *m_audioController; }

		[[nodiscard]] const AudioCreator* getAudioCreator() const noexcept;

	private:
		std::unique_ptr<AudioController> m_audioController;
	};
}