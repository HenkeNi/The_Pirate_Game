#pragma once
#include "engine/resources/resource_types.h"
#include <filesystem>
#include <memory>

namespace cursed_engine
{	
	template <typename T>
	class Result;

	class Audio;
	class AudioCreator;
	class Font;
	class Texture;
	class TextureCreator;

	struct AudioDescriptor;
	struct FontDescriptor;
	struct TextureDescriptor;

	class AudioLoader
	{
	public:
		virtual ~AudioLoader() = default;
		[[nodiscard]] virtual Result<AudioPtr> operator()(const AudioDescriptor& descriptor) const = 0;
	};

	class FontLoader
	{
	public:
		virtual ~FontLoader() = default;
		[[nodiscard]] virtual Result<FontPtr> operator()(const FontDescriptor& descriptor) const = 0;
	};

	class TextureLoader
	{
	public:
		virtual ~TextureLoader() = default;
		[[nodiscard]] virtual Result<TexturePtr> operator()(const TextureDescriptor& descriptor) const = 0;
	};

	struct Surface;

	// SDLSurface?
	struct SurfaceLoader
	{
		[[nodiscard]] Result<Surface> operator()(const std::filesystem::path& path) const;
	};

#pragma region SDL_Resource_Loaders

	class SDLAudioLoader final : public AudioLoader
	{
	public:
		explicit SDLAudioLoader(const AudioCreator* creator);
		[[nodiscard]] Result<AudioPtr> operator()(const AudioDescriptor& descriptor) const override;

	private:
		const AudioCreator* m_creator;
	};

	class SDLFontLoader final : public FontLoader
	{
	public:
		[[nodiscard]] Result<FontPtr> operator()(const FontDescriptor& descriptor) const override;
	};

	class SDLTextureLoader final : public TextureLoader
	{
	public:
		explicit SDLTextureLoader(const TextureCreator* creator);
		[[nodiscard]] Result<TexturePtr> operator()(const TextureDescriptor& descriptor) const;

	private:
		const TextureCreator* m_creator;
	};

#pragma endregion
}