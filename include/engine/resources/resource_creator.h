#pragma once
#include <string>

struct TTF_TextEngine;
struct SDL_Renderer;

namespace cursed_engine
{
	class Texture;
	class Text;
	class Font;
	struct Surface;

	// TODO; return Result?

#pragma region Resource_Creator

	class ResourceCreator
	{
	public:
		virtual ~ResourceCreator() = default;

		[[nodiscard]] virtual Texture createTextureFromSurface(Surface surface) const noexcept = 0;
		[[nodiscard]] virtual Text createText(const std::string& text, Font& font) const noexcept = 0;
	};

#pragma endregion

#pragma region SDL_Resource_Creator

	class SDLResourceCreator : public ResourceCreator
	{
	public:
		void init(TTF_TextEngine* textEngine, SDL_Renderer* renderer);

		[[nodiscard]] Texture createTextureFromSurface(Surface surface) const noexcept override;
		[[nodiscard]] Text createText(const std::string& text, Font& font) const noexcept override;


	private:
		TTF_TextEngine* m_textEngine;
		SDL_Renderer* m_renderer;
	};

#pragma endregion
}