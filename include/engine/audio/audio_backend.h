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
	class SDLAudioCreator;

	enum class AudioType : uint8_t;

	// Tie different ways of handling no available tracks (wait/queue, drop, replace, etc) to AudioType?

	class AudioBackend
	{
	public:
		AudioBackend() = default;
		virtual ~AudioBackend() = default;

		AudioBackend(const AudioBackend&) = delete;
		AudioBackend(AudioBackend&&) = delete;

		AudioBackend& operator=(const AudioBackend&) = delete;
		AudioBackend& operator=(AudioBackend&&) = delete;

		virtual Result<void> init() = 0;
		virtual void shutdown() {};

		virtual AudioPlaybackHandle play(const Audio& audio, AudioType type) = 0;

		virtual void pause(AudioPlaybackHandle handle) = 0;
		virtual void pauseAll() = 0;

		virtual void stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs = 0) = 0;
		virtual void stopAll(uint64_t fadeOutInMs = 0) = 0;

		virtual void setMasterVolume(float volume) = 0;
		virtual void setVolume(AudioType type, float volume) = 0;

		virtual void mute() = 0;
		virtual void unmute() = 0;

		[[nodiscard]] virtual bool isPlaying(AudioPlaybackHandle handle) const = 0;
		[[nodiscard]] virtual bool isPaused(AudioPlaybackHandle handle) const = 0;

		[[nodiscard]] virtual const AudioCreator* getAudioCreator() const noexcept { return nullptr; }

	};

	class SDLAudioBackend final : public AudioBackend
	{
	public:
		SDLAudioBackend();
		~SDLAudioBackend() = default;

		Result<void> init() override;
		void shutdown() override;

		AudioPlaybackHandle play(const Audio& audio, AudioType type) override;

		void pause(AudioPlaybackHandle handle) override;
		void pauseAll() override;

		void stop(AudioPlaybackHandle handle, uint64_t fadeOutInMs) override;
		void stopAll(uint64_t fadeOutInMs) override;

		void setMasterVolume(float volume) override;
		void setVolume(AudioType type, float volume) override;

		void mute() override;
		void unmute() override;

		[[nodiscard]] bool isPlaying(AudioPlaybackHandle handle) const override;
		[[nodiscard]] bool isPaused(AudioPlaybackHandle handle) const override;

		[[nodiscard]] const SDLAudioCreator* getAudioCreator() const noexcept override;

	private:
		struct Track
		{
			MIX_Track* internal{};
			uint32_t id{};
			uint32_t generation{};

			mutable std::atomic<bool> inUse{ false }; // ok to be mutable?		
		};

		void initializeTracks(AudioType type, int amount);
		const Track* getAvailableTrack(AudioType type) const;

		static void SDLCALL onTrackStopped(void* userdata, MIX_Track* mixTrack);
		void releaseTrack();

		static constexpr std::size_t toIndex(AudioType type) { return static_cast<std::size_t>(type); }

		const Track* resolve(AudioPlaybackHandle handle) const;
		Track* resolve(AudioPlaybackHandle handle);

		SDLAudioCreator m_audioCreator;
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