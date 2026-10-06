#pragma once

struct MIX_Audio;

namespace cursed_engine
{
	struct AudioDescriptor
	{
		std::string path;

		bool operator==(const AudioDescriptor& other) const noexcept
		{
			return path == other.path;
		}
	};

	class Audio
	{
	public:
		Audio() = default;
		virtual ~Audio() = default;

		Audio(const Audio&) = delete;
		Audio& operator=(const Audio&) = delete;

		[[nodiscard]] virtual float getDuration() const = 0;
	};

	class SDLAudio final : public Audio
	{
	public:
		SDLAudio(MIX_Audio* audio = nullptr);
		~SDLAudio();

		SDLAudio(SDLAudio&& other) noexcept;
		SDLAudio& operator=(SDLAudio&& other) noexcept;

		[[nodiscard]] inline MIX_Audio* getInternal() const noexcept { return m_audio; }
		[[nodiscard]] float getDuration() const override;

	private:
		MIX_Audio* m_audio;
	};
}

template<>
struct std::hash<cursed_engine::AudioDescriptor>
{
	std::size_t operator()(const cursed_engine::AudioDescriptor& key) const noexcept
	{
		return std::hash<std::string>{}(key.path);
	}
};