#pragma once

struct SDL_AudioStream;

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
		virtual ~Audio() = default;
	};

	class SDLAudio : public Audio
	{
	public:
		//Audio(const SDL_AudioSpec& spec, uint8_t* buffer, uint32_t length);
		SDLAudio();
		SDLAudio(SDL_AudioStream* stream, uint8_t* buffer, uint32_t length);

		SDL_AudioStream* m_stream;
		//SDL_AudioSpec m_spec; // use the one in audio controller...
		uint8_t* m_buffer; // or data
		uint32_t m_length;
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