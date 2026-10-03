#pragma once

//struct SDL_AudioStream;
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
	};

	class SDLAudio final : public Audio
	{
	public:
		SDLAudio(MIX_Audio* audio = nullptr);
		~SDLAudio();

		SDLAudio(SDLAudio&& other) noexcept;
		SDLAudio& operator=(SDLAudio&& other) noexcept;

		[[nodiscard]] inline MIX_Audio* getInternal() const noexcept { return m_audio; }

	private:
		MIX_Audio* m_audio;
	};

	//	SDL_AudioStream* m_stream;
	//	//SDL_AudioSpec m_spec; // use the one in audio controller...
	//	uint8_t* m_buffer; // or data
	//	uint32_t m_length;
	//};	
}

template<>
struct std::hash<cursed_engine::AudioDescriptor>
{
	std::size_t operator()(const cursed_engine::AudioDescriptor& key) const noexcept
	{
		return std::hash<std::string>{}(key.path);
	}
};