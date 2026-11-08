#pragma once
#include "engine/math/vec2.hpp"

typedef struct SDL_Window SDL_Window;
union SDL_Event;

namespace cursed_engine
{
	// TODO; send event, resize, etc? TODO; set min/max aspect ratio?

	struct Surface;

	template <typename T>
	class Result;

	struct WindowArgs
	{
		std::string title{};
		IVec2 position{}; // not used...
		int width{};
		int height{};
		bool fullscreen{};  // not used...
		bool alwaysOnTop{}; // not used...
	};

	class Window final
	{
	public:		
		struct State
		{
			IVec2 position{};
			IVec2 size{};

			bool alwaysOnTop = false;
			bool fullscreen = false;
			bool minimized = false;
			bool focused = true;
		};

		Window();
		~Window();

		Window(const Window&) = delete;
		Window(Window&& other) = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&& other) = delete;

		// ==================== Lifecycle ====================
		Result<void> init(const WindowArgs& args);
		void destroy();

		// ==================== Event Processing ====================
		void processEvent(const SDL_Event& event);

		// ==================== Getters ====================
		[[nodiscard]] void* getNativeHandle() const noexcept;
		[[nodiscard]] inline const State& getState() const noexcept { return m_state; }

		// ==================== Setters ====================
		bool setTitle(const char* title);
		bool setIcon(const Surface& surface);

		bool setAlwaysOnTop(bool enable);
		bool setFullscreen(bool enable);

		bool setPosition(int x, int y);
		bool setSize(int width, int height);

	private:
		SDL_Window* m_window = nullptr;
		State m_state;
	};
}