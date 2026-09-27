#include "engine/utils/frame_timer.h"
#include <SDL3/SDL.h>
#include <cassert> // remove later

#include <chrono>

namespace cursed_engine
{
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

	void FrameTimer::tick()
	{
		++m_currentFrame;

		auto now = std::chrono::steady_clock::now();
		m_deltaTime = std::chrono::duration<float>(now - lastTime).count();
		lastTime = now;

		/*m_totalTime += SDL_GetPerformanceCounter();
		m_current = SDL_GetPerformanceCounter();
		
		m_deltaTime = static_cast<double>(m_current - m_previous) / SDL_GetPerformanceFrequency();
	
		m_fps = 1.0f / m_deltaTime;
		m_previous = m_current;*/
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