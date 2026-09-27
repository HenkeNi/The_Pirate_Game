#pragma once
#include <string>

struct TTF_TextEngine;
struct SDL_Renderer;

namespace cursed_engine
{
	class Font;
	struct Surface;
	class Texture;
	class Text;

	template <typename T>
	class Result;

	// Rename, ResourceGenerator?

#pragma region Resource_Creator

	class ResourceCreator
	{
	public:
		virtual ~ResourceCreator() = default;

		[[nodiscard]] virtual Result<Texture> createTextureFromSurface(Surface surface) const = 0;
		[[nodiscard]] virtual Result<Text> createText(const std::string& text, Font& font) const = 0;
	};

#pragma endregion

#pragma region SDL_Resource_Creator

	class SDLResourceCreator : public ResourceCreator
	{
	public:
		void init(TTF_TextEngine* textEngine, SDL_Renderer* renderer);

		[[nodiscard]] Result<Texture> createTextureFromSurface(Surface surface) const override;
		[[nodiscard]] Result<Text> createText(const std::string& text, Font& font) const override;


	private:
		TTF_TextEngine* m_textEngine;
		SDL_Renderer* m_renderer;
	};

#pragma endregion
}