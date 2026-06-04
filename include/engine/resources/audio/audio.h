#pragma once
#include "engine/resources/resource_manager.hpp"

struct SDL_AudioStream;

namespace cursed_engine
{
	class Audio
	{
	public:
		struct Tag {}; // not used anymore???

		Audio();
		Audio(SDL_AudioStream* stream, uint8_t* buffer, uint32_t length);
		//Audio(const SDL_AudioSpec& spec, uint8_t* buffer, uint32_t length);
		~Audio();

		//void init(const SDL_AudioSpec& spec, uint8_t* buffer, uint32_t length);



	public:
		//private:
		SDL_AudioStream* m_stream;
		//SDL_AudioSpec m_spec; // use the one in audio controller...
		uint8_t* m_buffer; // or data
		uint32_t m_length;
	};


	struct AudioDescriptor
	{
		std::string path;

		bool operator==(const AudioDescriptor& other) const noexcept
		{
			return path == other.path;
		}
	};

	struct AudioLoader
	{
		[[nodiscard]] Audio operator()(const AudioDescriptor& key) const;
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