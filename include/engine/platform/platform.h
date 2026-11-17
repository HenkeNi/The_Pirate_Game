#pragma once
#include "engine/platform/cursor.h"
#include "engine/platform/input_handler.h"
#include "engine/platform/window.h"
#include <filesystem>
#include <functional>

union SDL_Event;

namespace cursed_engine
{
	struct EngineConfig;
	class EventBus;

	using NativeEventCallback = std::function<void(const SDL_Event&)>;
	using CallbackHandle = uint32_t;

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

		CallbackHandle addCallback(NativeEventCallback callback);
		void removeCallback(CallbackHandle handle);

		[[nodiscard]] bool exitRequested() const noexcept;
		[[nodiscard]] Window& getWindow() noexcept;

		[[nodiscard]] Cursor& getCursor() noexcept;
		[[nodiscard]] InputHandler& getInputHandler() noexcept;

	private:
		struct CallbackEntry
		{
			CallbackHandle handle;
			NativeEventCallback callback;
		};

		Window m_window;
		Cursor m_cursor;

		InputHandler m_inputHandler;
		EventBus& m_eventBus;

		std::vector<CallbackEntry> m_callbacks;
		CallbackHandle m_nextCallbackHandle;

		bool m_shouldExit;
	};
}