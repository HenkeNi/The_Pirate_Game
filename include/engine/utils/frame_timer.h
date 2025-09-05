#pragma once

namespace cursed_engine
{
	class FrameTimer
	{
	public:
		void tick();

		[[nodiscard]] double getDeltaTime() const; // noexecept?

		[[nodiscard]] double getFPS() const;

	private:
		//Uint64 m_last = SDL_GetPerformanceCounter();

		double m_totalTime = 0.0;

		double m_fps = 0.0;
		double m_deltaTime = 0.0;
	};
}