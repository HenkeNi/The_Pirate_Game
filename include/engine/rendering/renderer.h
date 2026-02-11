#pragma once
#include "engine/core/subsystem.h"
#include "engine/math/vec2.h" // TODO; put in pch...

typedef struct SDL_Renderer SDL_Renderer;

// TODO; handle metadata?? objects rendered, etc...

namespace cursed_engine
{
	class Window;
	class Texture;

	class Renderer : public Subsystem
	{
	public:
		Renderer();

		bool init(Window& window);
		void shutdown();

		void clearScreen();
		void present(); // rename?

		void renderTexture(FVec2 pos, FVec2 size, Texture& texture);
		void renderTexture(float x, float y, float width, float height, Texture& texture);

		void renderLine(FVec2 start, FVec2 end);
		void renderLine(float startX, float startY, float endX, float endY);

		[[nodiscard]] inline SDL_Renderer* getRenderer() noexcept { return m_renderer; }

	private:
		SDL_Renderer* m_renderer;
	};
}