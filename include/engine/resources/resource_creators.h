#pragma once
#include "engine/resources/resource_types.h"
#include <string>

struct MIX_Mixer;
struct SDL_Renderer;

namespace cursed_engine
{
	struct Surface;
	
	template <typename T>
	class Result;

	class AudioCreator final
	{
	public:
		void init(MIX_Mixer* mixer);

		[[nodiscard]] Result<AudioPtr> createAudio(const char* path) const;

	private:
		MIX_Mixer* m_mixer = nullptr;
	};

	class TextureCreator final
	{
	public:
		void init(SDL_Renderer* renderer);
		[[nodiscard]] Result<TexturePtr> createTextureFromSurface(Surface surface) const;

	private:
		SDL_Renderer* m_renderer = nullptr;
	};
}