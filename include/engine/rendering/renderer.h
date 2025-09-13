#pragma once

typedef struct SDL_Renderer SDL_Renderer;

namespace cursed_engine
{
	class Window;
	class Texture;

	class Renderer //: public Subsystem
	{
	public:
		Renderer();

		bool init(Window& window);
		void shutdown();

		void clearScreen();
		void present(); // rename?

		void renderTexture(float x, float y, Texture& texture);

		[[nodiscard]] inline SDL_Renderer* getRenderer() noexcept { return m_renderer; }

	private:
		SDL_Renderer* m_renderer;
	};
}