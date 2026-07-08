#pragma once

namespace cursed_engine
{
	class ResourceCreator;
	class Texture;
	struct TextureDescriptor;

	/*class ITextureLoader
	{
		[[nodiscard]] virtual Texture operator()(const TextureDescriptor& descriptor) const = 0;
	};*/

	// rename SDLTextureLoader
	class TextureLoader // : public ITextureLoader
	{
	public:
		explicit TextureLoader(ResourceCreator* creator);

		[[nodiscard]] Texture operator()(const TextureDescriptor& descriptor) const;

	private:
		ResourceCreator* m_creator;
	};
}