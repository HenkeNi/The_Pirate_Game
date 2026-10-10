#include "engine/audio/audio_controller.h"
#include "engine/audio/audio_backend.h"

namespace cursed_engine
{
	AudioController::AudioController(AudioBackend* backend)
		: m_backend{ backend }
	{
	}

	AudioPlaybackHandle AudioController::play(const Audio& audio, AudioType type)
	{
		return m_backend->play(audio, type);
	}

	void AudioController::pause(AudioPlaybackHandle handle)
	{ 
		m_backend->pause(handle);
	}

	void AudioController::pauseAll()
	{
		m_backend->pauseAll();
	}

	void AudioController::stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs)
	{
		m_backend->stop(handle);
	}

	void AudioController::stopAll(uint64_t fadeOutInMs)
	{
		m_backend->stopAll(fadeOutInMs);
	}	

	void AudioController::setMasterVolume(float volume)
	{ 
		m_backend->setMasterVolume(volume);
	}

	void AudioController::setVolume(AudioType type, float volume)
	{
		m_backend->setVolume(type, volume);
	}

	void AudioController::mute()
	{
		m_backend->mute();
	}

	void AudioController::unmute()
	{
		m_backend->unmute();
	}

	bool AudioController::isPlaying(AudioPlaybackHandle handle) const
	{
		return m_backend->isPlaying(handle);
	}

	bool AudioController::isPaused(AudioPlaybackHandle handle) const
	{
		return m_backend->isPaused(handle);
	}
}