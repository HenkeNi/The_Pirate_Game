#pragma once
#include "engine/audio/audio_types.h" // or forward declare

namespace cursed_engine
{
	class Audio;
	class AudioBackend;

	class AudioController
	{
	public:
		AudioController() = default;
		AudioController(AudioBackend* backend);

		AudioPlaybackHandle play(const Audio& audio, AudioType type);

		void pause(AudioPlaybackHandle handle);
		void pauseAll();

		void stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs);
		void stopAll(uint64_t fadeOutInMs);

		void setMasterVolume(float volume);
		void setVolume(AudioType type, float volume);

		void mute();
		void unmute();

		[[nodiscard]] bool isPlaying(AudioPlaybackHandle handle) const;
		[[nodiscard]] bool isPaused(AudioPlaybackHandle handle) const;

	private:
		AudioBackend* m_backend;
	};
}