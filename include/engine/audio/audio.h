#pragma once
#include "engine/resources/resource_manager.hpp"
#include <SDL3/SDL_audio.h> // dont include?

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


	struct AudioKey
	{
		std::string path;

		bool operator==(const AudioKey& other) const noexcept
		{
			return path == other.path;
		}
	};

	struct AudioLoader
	{
		[[nodiscard]] Audio operator()(const AudioKey& key) const;
	};

	using AudioManager = ResourceManager<Audio, AudioKey, AudioLoader>;
}

template<>
struct std::hash<cursed_engine::AudioKey>
{
	std::size_t operator()(const cursed_engine::AudioKey& key) const noexcept
	{
		return std::hash<std::string>{}(key.path);
	}
};