#pragma once
#include <SDL3/SDL_audio.h>

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
}