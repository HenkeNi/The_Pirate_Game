#pragma onc 
#include "../subsystems/subsystem.h"

typedef struct SDL_Renderer SDL_Renderer;

namespace cursed_engine
{
	class Window;

	class Renderer //: public Subsystem
	{
	public:
		Renderer();

		bool init(Window& window);
		void shutdown();

		void clearScreen();
		void present(); // rename?

	private:
		SDL_Renderer* m_renderer;
	};
}