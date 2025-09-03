#pragma once

struct SDL_Texture;

namespace cursed_engine
{
	class Texture
	{
	public:
		Texture(SDL_Texture* texture = nullptr);
		Texture(const Texture& other);
		Texture(Texture&& texture);
		~Texture();

		Texture& operator=(const Texture& other);
		Texture& operator=(Texture&& other);

		void init(SDL_Texture* texture);
		void destroy(); // or rename; "release"?

		[[nodiscard]] inline float getWidth() const noexcept { return m_width; }
		[[nodiscard]] inline float getHeight() const noexcept { return m_height; }

		bool isLoaded() const;

		//[[nodiscard]] inline const SDL_Texture* getTexture() const { return m_texture; }
		[[nodiscard]] inline SDL_Texture* getTexture() { return m_texture; }

	private:
		SDL_Texture* m_texture;
		float m_width;
		float m_height;
	};
}