#pragma once
#include "engine/platform/input/input_handler.h"
#include "engine/platform/window/window.h"
#include "engine/utils/frame_timer.h"

namespace cursed_engine
{
	struct EngineConfig;
	class EventBus; 

	class PlatformModule
	{  
	public:
		PlatformModule(EventBus& eventBus);

		bool init(const EngineConfig& config);
		void shutdown();

		void beginFrame();
		void endFrame(); // or swap buffers?

		void pollEvents();

		// get time?. get deltaTime()

		// should have these? or fetch window, etc??
		[[nodiscard]] inline bool shouldQuit() const noexcept { return !m_isRunning; }

		[[nodiscard]] inline float getFPS() const noexcept { return m_fps; }

		[[nodiscard]] double getDeltaTime() const noexcept;

		[[nodiscard]] uint64_t getFrameCount() const noexcept;

		[[nodiscard]] inline InputHandler& getInputHandler() noexcept { return m_inputHandler; }
		[[nodiscard]] inline FrameTimer& getFrameTimer() noexcept { return m_timer; }
		[[nodiscard]] inline Window& getWindow() noexcept { return m_window; }

		// get frame stats?

	private:
		InputHandler m_inputHandler;
		FrameTimer m_timer;
		Window m_window;
		// Timer class?
		uint64_t m_frameBeginCounter;
		float m_fps;
		bool m_isRunning;
	};
}