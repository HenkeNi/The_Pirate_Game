#include "engine/audio/audio_controller.h"
#include "engine/core/result.h"
#include "engine/core/logger.h"
#include "engine/resources/audio/audio.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_audio.h>
#include <format>

namespace
{
	constexpr int NUMBER_OF_TRACKS_AT_START = 16;
}

namespace cursed_engine
{
	//AudioController::AudioController()
	//	: m_audioStream{ nullptr }, m_deviceId{ 0 }
	//{
	//}

	//Result<void> AudioController::init()
	//{
	//	static SDL_AudioDeviceID audio_device = 0;

	//	// TODO; use same spec for audio controller, and audio's?
	//	SDL_AudioSpec spec;
	//	SDL_zero(spec);
	//	spec.freq = 48000;
	//	spec.format = SDL_AUDIO_S16LE;
	//	spec.channels = 2;

	//	//m_deviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

	//	m_audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

	//	if (!m_audioStream)
	//	{
	//		return Result<void>::failure(std::format("Failed to open audio stream! Error: {}", SDL_GetError()));
	//	}

	//	m_deviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

	//	SDL_ResumeAudioStreamDevice(m_audioStream);
	//	return Result<void>::success();
	//}

	//void AudioController::shutdown()
	//{
	//	// TODO!
	//}

	//void AudioController::playSound(SDL_AudioStream* stream, uint8_t* buffer, uint32_t length)
	//{
	//	static bool test = false;

	//	if (!test)
	//	{

	//		// DO in init sound?
	//		if (!SDL_BindAudioStream(m_deviceId, stream))
	//		{
	//			Logger::logError(std::format("Failed to bind stream! Error: {}", SDL_GetError()));

	//		}

	//		test = true;
	//	}




	//	// THIS Checks if sound can be queued??? -> have own queue if fails?
	//	if (SDL_GetAudioStreamQueued(m_audioStream) < (int)length) {
	//		/* feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
	//		SDL_PutAudioStreamData(m_audioStream, buffer, length);
	//	}


	//	//if (!SDL_PutAudioStreamData(m_audioStream, buffer, length))
	//	//{
	//	//	Logger::logWarning("Failed to put audio stream"); // TOOD; log error to
	//	//}


	//	//SDL_BindAudioStream(m_deviceId, m_audioStream);
	//	//SDL_ResumeAudioDevice(m_deviceId);
	//}

	///*SDL_AudioSpec AudioController::getSpecs()
	//{
	//	return SDL_AudioSpec();
	//}*/

	SDLAudioController::SDLAudioController()
		: m_mixer{ nullptr }
	{
	}

	Result<void> SDLAudioController::init()
	{ 
		if (!MIX_Init())
		{
			return Result<void>::failure(
				std::format("Failed to initialize SDL3-Mixer! Reason: {}", SDL_GetError()));
		}

		m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

		if (!m_mixer)
		{
			return Result<void>::failure(
				std::format("Failed to create mixer! Reason: {}", SDL_GetError()));
		}

		generateTracks(NUMBER_OF_TRACKS_AT_START);
		m_audioCreator.init(m_mixer);

		return Result<void>::success();
	}
	
	void SDLAudioController::shutdown()
	{

	}

	void SDLAudioController::play(const Audio& audio)
	{
		//const SDLAudio& sdlAudio = static_cast<const SDLAudio&>(audio); // TEMP!!
		//MIX_SetTrackAudio(m_tracks[0], sdlAudio.getInternal()); // friend class instead? wont have to pass const ref...
		//MIX_PlayTrack(m_tracks[0], 0);
	}

	void SDLAudioController::pause(const Audio& audio)
	{

	}

	void SDLAudioController::stop(const Audio& audio)
	{

	}

	const SDLAudioCreator* SDLAudioController::getAudioCreator() const noexcept 
	{ 
		return &m_audioCreator; 
	}

	void SDLAudioController::generateTracks(int amount)
	{ 
		for (int i = 0; i < amount; ++i)
		{
			m_tracks.push_back(MIX_CreateTrack(m_mixer));
		}
	}
}