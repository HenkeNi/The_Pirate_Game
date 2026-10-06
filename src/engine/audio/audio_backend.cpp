#include "engine/audio/audio_backend.h"
#include "engine/audio/audio_types.h"
#include "engine/resources/audio/audio.h"
#include "engine/core/result.h"
#include <algorithm>
#include <format>

namespace
{
	constexpr int NUMBER_OF_SFX_TRACKS = 32;
	constexpr int NUMBER_OF_MUSIC_TRACKS = 2;
	constexpr int NUMBER_OF_AMBIENCE_TRACKS = 8;
	constexpr int NUMBER_OF_VOICE_TRACKS = 4;
	constexpr int NUMBER_OF_UI_TRACKS = 6;
	constexpr float MAX_VOLUME = 1.0f;

	constexpr uint32_t INVALID_GENERATION = 0;

	struct TrackCallbackData
	{
		cursed_engine::SDLAudioBackend* backend;
		cursed_engine::AudioType type{};
		std::size_t index{};
	};
}

namespace cursed_engine
{
	SDLAudioBackend::SDLAudioBackend()
		: m_mixer{ nullptr }
	{
	}

	Result<void> SDLAudioBackend::init()
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

		initializeTracks(AudioType::Sfx, NUMBER_OF_SFX_TRACKS);
		initializeTracks(AudioType::Music, NUMBER_OF_MUSIC_TRACKS);
		initializeTracks(AudioType::Ambience, NUMBER_OF_AMBIENCE_TRACKS);
		initializeTracks(AudioType::Voice, NUMBER_OF_VOICE_TRACKS);
		initializeTracks(AudioType::UI, NUMBER_OF_UI_TRACKS);

		m_audioCreator.init(m_mixer);

		return Result<void>::success();
	}

	void SDLAudioBackend::shutdown()
	{
		MIX_DestroyMixer(m_mixer);
		m_mixer = nullptr;

		MIX_Quit();
	}

	AudioPlaybackHandle SDLAudioBackend::play(const Audio& audio, AudioType type)
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

	void SDLAudioBackend::pause(AudioPlaybackHandle handle)
	{
		if (Track* track = resolve(handle))
		{
			MIX_PauseTrack(track->internal);
		}
	}

	void SDLAudioBackend::pauseAll()
	{
		MIX_PauseAllTracks(m_mixer);
	}

	void SDLAudioBackend::stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs)
	{
		if (Track* track = resolve(handle))
		{
			MIX_StopTrack(track->internal, fadeOutInMs);
		}
	}

	void SDLAudioBackend::stopAll(uint64_t fadeOutInMs)
	{
		MIX_StopAllTracks(m_mixer, fadeOutInMs);
	}

	void SDLAudioBackend::setMasterVolume(float volume)
	{
		volume = std::clamp(volume, 0.f, MAX_VOLUME);
		MIX_SetMixerGain(m_mixer, volume);
	}

	void SDLAudioBackend::setVolume(AudioType type, float volume)
	{
		volume = std::clamp(volume, 0.f, MAX_VOLUME);

		TrackPool& pool = m_trackPools.at(static_cast<std::size_t>(type));
		
		for (Track& track : pool.tracks())
		{
			MIX_SetTrackGain(track.internal, volume);
		}
	}

	void SDLAudioBackend::mute()
	{
		assert(false && "SDLAudioBackend::mute() is not implemented!");
		// requires storing volumes for each audio type (so it can be restored later?)
	}

	void SDLAudioBackend::unmute()
	{
		assert(false && "SDLAudioBackend::unmute() is not implemented!");
	}

	bool SDLAudioBackend::isPlaying(AudioPlaybackHandle handle) const
	{
		if (const Track* track = resolve(handle))
		{
			return MIX_TrackPlaying(track->internal);
		}
		
		return false;
	}

	bool SDLAudioBackend::isPaused(AudioPlaybackHandle handle) const
	{
		if (const Track* track = resolve(handle))
		{
			return MIX_TrackPaused(track->internal);
		}

		return false;
	}

	const SDLAudioCreator* SDLAudioBackend::getAudioCreator() const noexcept
	{
		return &m_audioCreator;
	}

	void SDLAudioBackend::initializeTracks(AudioType type, int amount)
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

			MIX_SetTrackStoppedCallback(track.internal, &SDLAudioBackend::onTrackStopped, &track);
		}
	}

	const SDLAudioBackend::Track* SDLAudioBackend::getAvailableTrack(AudioType type) const
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

	void SDLAudioBackend::onTrackStopped(void* userdata, MIX_Track* mixTrack)
	{
		Track* track = static_cast<Track*>(userdata);
		track->inUse.store(false);
	}

	void SDLAudioBackend::releaseTrack()
	{
	}

	const SDLAudioBackend::Track* SDLAudioBackend::resolve(AudioPlaybackHandle handle) const
	{
		const TrackPool& pool = m_trackPools.at(toIndex(handle.audioType));
		
		if (handle.id < 0 || handle.id >= pool.tracks().size())
			return nullptr;

		const Track& track = pool.tracks()[(std::size_t)handle.id]; // TODO; store handle.id as std::size_t?

		return track.generation == handle.generation ? &track : nullptr;
	}

	SDLAudioBackend::Track* SDLAudioBackend::resolve(AudioPlaybackHandle handle)
	{
		return const_cast<SDLAudioBackend::Track*>(std::as_const(*this).resolve(handle));
	}
}