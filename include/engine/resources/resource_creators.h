#pragma once
#include <memory>
#include <string>

struct MIX_Mixer;
struct SDL_Renderer;
struct TTF_TextEngine;

namespace cursed_engine
{
	class Audio;
	class Font;
	class Text;
	class Texture;
	struct Surface;

	template <typename T>
	class Result;

	class AudioCreator
	{
	public:
		virtual ~AudioCreator() = default;

		[[nodiscard]] virtual Result< std::unique_ptr<Audio>> createAudio(const char* path) const = 0;
	};

	class TextCreator
	{
	public:
		virtual ~TextCreator() = default;

		[[nodiscard]] virtual Result<Text> createText(const std::string& text, Font& font) const = 0;
	};

	class TextureCreator
	{
	public:
		virtual ~TextureCreator() = default;

		[[nodiscard]] virtual Result<std::unique_ptr<Texture>> createTextureFromSurface(Surface surface) const = 0;
	};

#pragma region SDL_Resource_Creators

	class SDLAudioCreator final : public AudioCreator
	{
	public:
		void init(MIX_Mixer* mixer);
		[[nodiscard]] Result<std::unique_ptr<Audio>> createAudio(const char* path) const override;
	
	private:
		MIX_Mixer* m_mixer = nullptr;
	};

	class SDLTextCreator final : public TextCreator
	{
	public:
		void init(TTF_TextEngine* textEngine);
		[[nodiscard]] Result<Text> createText(const std::string& text, Font& font) const override;

	private:
		TTF_TextEngine* m_textEngine = nullptr;
	};

	class SDLTextureCreator final : public TextureCreator
	{
	public:
		void init(SDL_Renderer* renderer);
		[[nodiscard]] Result<std::unique_ptr<Texture>> createTextureFromSurface(Surface surface) const override;

	private:
		SDL_Renderer* m_renderer = nullptr;
	};

#pragma endregion
}