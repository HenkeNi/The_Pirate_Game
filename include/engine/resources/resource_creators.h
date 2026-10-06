#pragma once
#include "engine/resources/resource_types.h"
#include <string>

struct MIX_Mixer;
struct SDL_Renderer;

namespace cursed_engine
{
	template <typename T>
	class Result;

	struct Surface;

	class AudioCreator
	{
	public:
		virtual ~AudioCreator() = default;

		[[nodiscard]] virtual Result<AudioPtr> createAudio(const char* path) const = 0;
	};

	class TextureCreator
	{
	public:
		virtual ~TextureCreator() = default;

		[[nodiscard]] virtual Result<TexturePtr> createTextureFromSurface(Surface surface) const = 0;
	};

#pragma region SDL_Resource_Creators

	class SDLAudioCreator final : public AudioCreator
	{
	public:
		void init(MIX_Mixer* mixer);
		[[nodiscard]] Result<AudioPtr> createAudio(const char* path) const override;
	
	private:
		MIX_Mixer* m_mixer = nullptr;
	};

	class SDLTextureCreator final : public TextureCreator
	{
	public:
		void init(SDL_Renderer* renderer);
		[[nodiscard]] Result<TexturePtr> createTextureFromSurface(Surface surface) const override;

	private:
		SDL_Renderer* m_renderer = nullptr;
	};

#pragma endregion
}