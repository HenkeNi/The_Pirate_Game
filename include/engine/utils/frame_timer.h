#pragma once

namespace cursed_engine
{
	class FrameTimer
	{
	public:
		void tick();

		[[nodiscard]] double getDeltaTime() const; // noexecept?

		[[nodiscard]] double getFPS() const;

		[[nodiscard]] inline uint64_t frameCount() const noexcept { return m_currentFrame; }

	private:
		//Uint64 m_last = SDL_GetPerformanceCounter();

		double m_totalTime = 0.0;

		double m_fps = 0.0;
		double m_deltaTime = 0.0;

		uint64_t m_currentFrame = 0; // store current "stats", fps, etc in struct? frametimer?		
	};
}