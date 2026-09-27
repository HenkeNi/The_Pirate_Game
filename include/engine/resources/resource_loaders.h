#pragma once

namespace cursed_engine
{
	template <typename T>
	class Result;
	
	class Audio;
	class Font;
	class Texture;

	struct AudioDescriptor;
	struct FontDescriptor;
	struct TextureDescriptor;

	class AudioLoader
	{
	public:
		virtual ~AudioLoader() = default;
		[[nodiscard]] virtual Result<Audio> operator()(const AudioDescriptor& descriptor) const = 0;
	};

	class FontLoader
	{
	public:
		virtual ~FontLoader() = default;
		[[nodiscard]] virtual Result<Font> operator()(const FontDescriptor& descriptor) const = 0;
	};

	class TextureLoader
	{
	public:
		virtual ~TextureLoader() = default;
		[[nodiscard]] virtual Result<Texture> operator()(const TextureDescriptor& descriptor) const = 0;
	};
}