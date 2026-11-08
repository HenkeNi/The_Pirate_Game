#pragma once
#include "engine/resources/resource_creators.h"
#include "engine/audio/audio_types.h"
#include <SDL3_mixer/SDL_mixer.h> // needed for SDLCALL (onTrackStopped) > consider making function 'free floating' in c.pp
#include <array>
#include <atomic>
#include <memory>
#include <span>
#include <vector>

struct MIX_Mixer;
struct MIX_Track;

namespace cursed_engine
{
	template <typename T>
	class Result;

	class Audio;
	class AudioCreator;

	enum class AudioType : uint8_t;

	// Tie different ways of handling no available tracks (wait/queue, drop, replace, etc) to AudioType?

	struct AudioBackendArgs
	{
		struct Options
		{
			float volume{};
			int tracks{};
		};

		std::array<Options, static_cast<std::size_t>(AudioType::Count)> audioOptions; // TODO; use array instead!
		float masterVolume{};
	};

	class AudioBackend final
	{
	public:
		AudioBackend();
		~AudioBackend();

		AudioBackend(const AudioBackend&) = delete;
		AudioBackend(AudioBackend&&) = delete;

		AudioBackend& operator=(const AudioBackend&) = delete;
		AudioBackend& operator=(AudioBackend&&) = delete;

		bool init(const AudioBackendArgs& args);
		void shutdown();

		AudioPlaybackHandle play(const Audio& audio, AudioType type);

		void pause(AudioPlaybackHandle handle);
		void pauseAll();

		void stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs = 0);
		void stopAll(uint64_t fadeOutInMs = 0);

		void setMasterVolume(float volume);
		void setVolume(AudioType type, float volume);

		void mute();
		void unmute();

		[[nodiscard]] bool isPlaying(AudioPlaybackHandle handle) const;
		[[nodiscard]] bool isPaused(AudioPlaybackHandle handle) const;

		[[nodiscard]] const AudioCreator* getAudioCreator() const noexcept;

	private:
		struct Track
		{
			MIX_Track* internal{};
			uint32_t id{};
			uint32_t generation{};

			mutable std::atomic<bool> inUse{ false };
		};

		void initializeTracks(AudioType type, int amount);
		const Track* getAvailableTrack(AudioType type) const;

		static void SDLCALL onTrackStopped(void* userdata, MIX_Track* mixTrack);
		void releaseTrack();

		static constexpr std::size_t toIndex(AudioType type) { return static_cast<std::size_t>(type); }

		const Track* resolve(AudioPlaybackHandle handle) const;
		Track* resolve(AudioPlaybackHandle handle);

		AudioCreator m_audioCreator;
		MIX_Mixer* m_mixer;

		// Use MIX_Group instead?
		class TrackPool
		{
		public:
			void init(std::size_t count)
			{
				m_tracks = std::make_unique<Track[]>(count);
				m_count = count;
			}

			std::span<const Track> tracks() const { return { m_tracks.get(), m_count }; }
			std::span<Track> tracks() { return { m_tracks.get(), m_count }; }

		private:
			std::unique_ptr<Track[]> m_tracks;
			std::size_t m_count;
		};

		std::array<TrackPool, static_cast<std::size_t>(AudioType::Count)> m_trackPools;
		std::array<float, static_cast<std::size_t>(AudioType::Count)> m_volumes;
	};
}