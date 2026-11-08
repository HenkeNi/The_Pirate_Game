#include "engine/audio/audio_backend.h"
#include "engine/audio/audio_types.h"

#include "engine/core/logger.h"

#include "engine/resources/audio/audio.h"

#include <algorithm>
#include <format>

namespace
{
	//constexpr int NUMBER_OF_SFX_TRACKS = 32;
	//constexpr int NUMBER_OF_MUSIC_TRACKS = 2;
	//constexpr int NUMBER_OF_AMBIENCE_TRACKS = 8;
	//constexpr int NUMBER_OF_VOICE_TRACKS = 4;
	//constexpr int NUMBER_OF_UI_TRACKS = 6;
	constexpr float MAX_VOLUME = 1.0f;

	constexpr uint32_t INVALID_GENERATION = 0;

	/*struct TrackCallbackData
	{
		cursed_engine::SDLAudioBackend* backend;
		cursed_engine::AudioType type{};
		std::size_t index{};
	};*/
}

namespace cursed_engine
{
	AudioBackend::AudioBackend()
		: m_mixer{ nullptr }
	{
	}

	AudioBackend::~AudioBackend()
	{
		shutdown();
	}

	bool AudioBackend::init(const AudioBackendArgs& args)
	{
		if (!MIX_Init())
		{
			Logger::logError(std::format("Failed to initialize SDL3-Mixer! Reason: {}", SDL_GetError()));
			return false;
		}

		m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

		if (!m_mixer)
		{
			Logger::logError(std::format("Failed to create mixer device! Reason: {}", SDL_GetError()));
			return false;
		}

		m_audioCreator.init(m_mixer);
		
		const auto& sfxOptions = args.audioOptions.at(static_cast<std::size_t>(AudioType::Sfx));
		initializeTracks(AudioType::Sfx, sfxOptions.tracks);
		setVolume(AudioType::Sfx, sfxOptions.volume);

		const auto& musicOptions = args.audioOptions.at(static_cast<std::size_t>(AudioType::Music));
		initializeTracks(AudioType::Music, musicOptions.tracks);
		setVolume(AudioType::Music, musicOptions.volume);

		const auto& ambienceOptions = args.audioOptions.at(static_cast<std::size_t>(AudioType::Ambience));
		initializeTracks(AudioType::Ambience, ambienceOptions.tracks);
		setVolume(AudioType::Ambience, ambienceOptions.volume);

		const auto& voiceOptions = args.audioOptions.at(static_cast<std::size_t>(AudioType::Voice));
		initializeTracks(AudioType::Voice, voiceOptions.tracks);
		setVolume(AudioType::Voice, voiceOptions.volume);

		const auto& uiOptions = args.audioOptions.at(static_cast<std::size_t>(AudioType::UI));
		initializeTracks(AudioType::UI, uiOptions.tracks);
		setVolume(AudioType::UI, uiOptions.volume);

		Logger::logInfo("\'Audio\' successfully initialized!");
		
		int version = MIX_Version();
		Logger::logInfo(std::format("{} ~ SDL_mixer version: {}.{}.{}",
			log_format::INDENT,
			SDL_VERSIONNUM_MAJOR(version),
			SDL_VERSIONNUM_MINOR(version),
			SDL_VERSIONNUM_MICRO(version)));

		return true;
	}

	void AudioBackend::shutdown()
	{
		MIX_DestroyMixer(m_mixer);
		m_mixer = nullptr;

		MIX_Quit();
	}

	AudioPlaybackHandle AudioBackend::play(const Audio& audio, AudioType type)
	{
		if (const Track* track = getAvailableTrack(type))
		{
			track->inUse.store(true);

			const SDLAudio& sdlAudio = static_cast<const SDLAudio&>(audio);
			MIX_Track* internal = track->internal;

			MIX_SetTrackAudio(internal, sdlAudio.getInternal());
			MIX_PlayTrack(internal, 0); // options (0)

			return AudioPlaybackHandle{ track->id, track->generation, type };
		}
		else
		{
			// TODO: handle non available tracks based on AudioType?

			return AudioPlaybackHandle::invalid();
		}
	}

	void AudioBackend::pause(AudioPlaybackHandle handle)
	{
		if (Track* track = resolve(handle))
		{
			MIX_PauseTrack(track->internal);
		}
	}

	void AudioBackend::pauseAll()
	{
		MIX_PauseAllTracks(m_mixer);
	}

	void AudioBackend::stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs)
	{
		if (Track* track = resolve(handle))
		{
			MIX_StopTrack(track->internal, fadeOutInMs);
		}
	}

	void AudioBackend::stopAll(uint64_t fadeOutInMs)
	{
		MIX_StopAllTracks(m_mixer, fadeOutInMs);
	}

	void AudioBackend::setMasterVolume(float volume)
	{
		volume = std::clamp(volume, 0.f, MAX_VOLUME);
		MIX_SetMixerGain(m_mixer, volume);
	}

	void AudioBackend::setVolume(AudioType type, float volume)
	{
		volume = std::clamp(volume, 0.f, MAX_VOLUME);

		TrackPool& pool = m_trackPools.at(static_cast<std::size_t>(type));
		
		for (Track& track : pool.tracks())
		{
			MIX_SetTrackGain(track.internal, volume);
		}
	}

	void AudioBackend::mute()
	{
		assert(false && "SDLAudioBackend::mute() is not implemented!");
		// requires storing volumes for each audio type (so it can be restored later?)
	}

	void AudioBackend::unmute()
	{
		assert(false && "SDLAudioBackend::unmute() is not implemented!");
	}

	bool AudioBackend::isPlaying(AudioPlaybackHandle handle) const
	{
		if (const Track* track = resolve(handle))
		{
			return MIX_TrackPlaying(track->internal);
		}
		
		return false;
	}

	bool AudioBackend::isPaused(AudioPlaybackHandle handle) const
	{
		if (const Track* track = resolve(handle))
		{
			return MIX_TrackPaused(track->internal);
		}

		return false;
	}

	const AudioCreator* AudioBackend::getAudioCreator() const noexcept
	{
		return &m_audioCreator;
	}

	void AudioBackend::initializeTracks(AudioType type, int amount)
	{
		TrackPool& pool = m_trackPools.at(static_cast<std::size_t>(type));
		pool.init(amount);

		for (int i = 0; i < amount; ++i)
		{
			Track& track = pool.tracks()[i];
			track.internal = MIX_CreateTrack(m_mixer);
			track.id = i;
			track.generation = INVALID_GENERATION;
			track.inUse.store(false);

			MIX_SetTrackStoppedCallback(track.internal, &AudioBackend::onTrackStopped, &track);
		}
	}

	const AudioBackend::Track* AudioBackend::getAvailableTrack(AudioType type) const
	{
		const TrackPool& pool = m_trackPools.at(static_cast<std::size_t>(type));

		auto it = std::find_if(pool.tracks().begin(), pool.tracks().end(), [](const Track& track) 
			{
				return !track.inUse.load(); // or use inUse.compare_exchange_strong(expected, true, std::memory_order_acquire)
			});

		if (it != pool.tracks().end())
			return &(*it);

		return nullptr;
	}

	void AudioBackend::onTrackStopped(void* userdata, MIX_Track* mixTrack)
	{
		Track* track = static_cast<Track*>(userdata);
		track->inUse.store(false);
	}

	void AudioBackend::releaseTrack()
	{
	}

	const AudioBackend::Track* AudioBackend::resolve(AudioPlaybackHandle handle) const
	{
		const TrackPool& pool = m_trackPools.at(toIndex(handle.audioType));
		
		if (handle.id < 0 || handle.id >= pool.tracks().size())
			return nullptr;

		const Track& track = pool.tracks()[(std::size_t)handle.id]; // TODO; store handle.id as std::size_t?

		return track.generation == handle.generation ? &track : nullptr;
	}

	AudioBackend::Track* AudioBackend::resolve(AudioPlaybackHandle handle)
	{
		return const_cast<AudioBackend::Track*>(std::as_const(*this).resolve(handle));
	}
}