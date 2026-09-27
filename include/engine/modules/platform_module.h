#pragma once
#include "engine/platform/input_api.h"
#include "engine/utils/frame_timer.h"
#include <memory>

namespace cursed_engine
{
	struct EngineConfig;
	class Platform;
	class EventBus; 
	class Window;
	class InputAPI;
	class Cursor;

	class PlatformModule
	{  
	public:
		PlatformModule(EventBus& eventBus);
		~PlatformModule();

		bool init(const EngineConfig& config);
		void shutdown();

		void beginFrame();
		void endFrame();

		void processEvents();

		// get time?. get deltaTime()
		// should have these? or fetch window, etc??
		[[nodiscard]] bool exitRequested() const noexcept;

		[[nodiscard]] double getDeltaTime() const noexcept;

		[[nodiscard]] uint64_t getFrameCount() const noexcept;

		//[[nodiscard]] inline float getDeltaTime() const noexcept { return m_deltaTime; }
		[[nodiscard]] inline float getFPS() const noexcept { return m_fps; }

		[[nodiscard]] Window& getWindow() noexcept;
		[[nodiscard]] Cursor& getCursor() noexcept;
		[[nodiscard]] InputAPI getInputAPI() noexcept;

		
		[[nodiscard]] inline FrameTimer& getFrameTimer() noexcept { return m_timer; }

		// get frame stats?
		// return current backend type?
		// cursor?

	private:
		std::unique_ptr<Platform> m_platform;
		EventBus& m_eventBus;
	
		// time system?
		FrameTimer m_timer; // dont put frame timer here? put in IMpl?
		
		uint64_t m_frameBeginCounter; // rename startFrame?
		float m_deltaTime;
		float m_fps;
		//bool m_isRunning; // use shouldQuit? store here or in platfomr?
	};
}