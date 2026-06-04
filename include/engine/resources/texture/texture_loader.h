#pragma once

namespace cursed_engine
{
	class Renderer;
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
		explicit TextureLoader(Renderer* renderer);

		[[nodiscard]] Texture operator()(const TextureDescriptor& descriptor) const;

	private:
		Renderer* m_renderer;
	};
}