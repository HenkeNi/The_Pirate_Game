#include "engine/utils/frame_timer.h"
#include <SDL3/SDL.h>

namespace cursed_engine
{
	void FrameTimer::tick()
	{
		++m_currentFrame;
		m_totalTime += SDL_GetPerformanceCounter();

		m_current = SDL_GetPerformanceCounter();
		m_deltaTime = static_cast<double>(m_current - m_previous) / SDL_GetPerformanceFrequency();
	
		m_fps = 1.0f / m_deltaTime;

		m_previous = m_current;
	}

	double FrameTimer::getDeltaTime() const
	{
		return m_deltaTime;
	}

	double FrameTimer::getFPS() const
	{
		return m_fps;
	}

}