#pragma once

namespace cursed_engine
{
	// put all in a Time.h?
	class Timer
	{
	public:
		Timer();

		void start();
		void stop();
		void pause();
		void resume();

		[[nodiscard]] bool isPaused() const noexcept;
		
		[[nodiscard]] bool isRunning() const noexcept;



	private:
		//uint64 m_startTime;
	};
}