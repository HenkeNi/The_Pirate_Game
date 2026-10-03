#pragma once
#include "engine/resources/resource_creators.h"
#include <vector>

struct MIX_Mixer;
struct MIX_Track;

namespace cursed_engine
{
	template <typename T>
	class Result;

	class AudioCreator;

	enum class AudioType
	{
		Ambience,
		Music,
		SFX
	};

	class AudioController
	{
	public:
		AudioController() = default;
		virtual ~AudioController() = default;

		AudioController(const AudioController&) = delete;
		AudioController(AudioController&&) = delete;

		AudioController& operator=(const AudioController&) = delete;
		AudioController& operator=(AudioController&&) = delete;

		virtual Result<void> init() = 0;
		virtual void shutdown() {};

		virtual void play(const Audio& audio) = 0;
		virtual void pause(const Audio& audio) = 0;
		virtual void stop(const Audio& audio) = 0;

		// mute?
		// volume
		// stop all?
		// stop all of certain "category"? VFX, 

		[[nodiscard]] virtual const AudioCreator* getAudioCreator() const noexcept { return nullptr; }

	};

	class SDLAudioController final : public AudioController
	{
	public:
		SDLAudioController();
		~SDLAudioController() = default;

		Result<void> init() override;
		void shutdown() override;

		void play(const Audio& audio) override; // accept SDLAudio? even possible?
		void pause(const Audio& audio) override;
		void stop(const Audio& audio) override;

		[[nodiscard]] const SDLAudioCreator* getAudioCreator() const noexcept override;

	private:
		void generateTracks(int amount);

		SDLAudioCreator m_audioCreator;
		MIX_Mixer* m_mixer;
		std::vector<MIX_Track*> m_tracks; // array?

		// map from audio type to tracks? .... SFX tracks ~16-32 .... Music tracks ~1 - 2
	};
}