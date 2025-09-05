#include "engine/utils/frame_timer.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	void FrameTimer::tick()
	{
		m_totalTime += SDL_GetPerformanceCounter();
	}

	double FrameTimer::getDeltaTime() const
	{
		return 0.0;
	}

	double FrameTimer::getFPS() const
	{
		return 0.0;
	}

}