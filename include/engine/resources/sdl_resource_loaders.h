#pragma once
#include "engine/resources/resource_loaders.h"
#include <filesystem>

namespace cursed_engine
{
	// [Consider] - whether inheritance is needed (or place loader as template argument?

	//class SDLResourceCreator;
	class ResourceCreator;

	class SDLAudioLoader : public AudioLoader
	{
	public:
		[[nodiscard]] Result<Audio> operator()(const AudioDescriptor& descriptor) const override;
	};

	class SDLFontLoader : public FontLoader
	{
	public:
		[[nodiscard]] Result<Font> operator()(const FontDescriptor& descriptor) const override;
	};

	struct Surface;

	// SDLSurface?
	struct SurfaceLoader
	{
		[[nodiscard]] Result<Surface> operator()(const std::filesystem::path& path) const;
	};

	class SDLTextureLoader : public TextureLoader
	{
	public:
		explicit SDLTextureLoader(ResourceCreator* creator);
		//explicit SDLTextureLoader(SDLResourceCreator* creator);

		[[nodiscard]] Result<Texture> operator()(const TextureDescriptor& descriptor) const;

	private:
		ResourceCreator* m_creator;
	};
}