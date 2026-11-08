#pragma once
#include "engine/platform/cursor.h"
#include "engine/platform/input_handler.h"
#include "engine/platform/window.h"
#include <filesystem>

namespace cursed_engine
{
	struct EngineConfig;
	class EventBus;

	struct PlatformArgs
	{
		WindowArgs wndArgs;
		InputArgs inputArgs;
		std::filesystem::path iconPath;
		const char* name;
		const char* version;
		const char* identifier;
	};

	class Platform final
	{
	public:
		Platform(EventBus& eventBus);
		~Platform();

		Platform(const Platform&) = delete;
		Platform(Platform&&) = delete;
		
		Platform& operator=(const Platform&) = delete;
		Platform& operator=(Platform&&) = delete;
		 
		bool init(const PlatformArgs& args);
		void shutdown();

		void beginFrame();
		void endFrame();
		
		void processEvents();

		[[nodiscard]] bool exitRequested() const noexcept;
		[[nodiscard]] Window& getWindow() noexcept;

		[[nodiscard]] Cursor& getCursor() noexcept;
		[[nodiscard]] InputHandler& getInputHandler() noexcept;

	private:
		Window m_window;
		Cursor m_cursor;

		InputHandler m_inputHandler;
		EventBus& m_eventBus;

		bool m_shouldExit;
	};
}