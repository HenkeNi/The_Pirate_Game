#pragma once
#include "engine/utils/non_copyable.h"
#include "engine/math/vec2.hpp"
#include <string>

typedef struct SDL_Window SDL_Window;
union SDL_Event;

namespace cursed_engine
{
	// TODO; Add a window builder?
	// TODO; handle multiple vsync "modes"?
	// TODO; test / make sure, set vsync, etc works!

	struct WindowConfig;
	struct Surface;

	class Window : public NonCopyable
	{
	public:
		Window();
		~Window();

		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		// ==================== Lifecycle ====================
		bool create(const char* title, const WindowConfig& config);
		void destroy();

		// ==================== Event Handling ====================
		void processEvent(const SDL_Event& event);
		
		// ==================== Queries ====================
		[[nodiscard]] inline SDL_Window* getHandle() const noexcept { return m_window; } // rename? getInternal?
		
		[[nodiscard]] inline const std::string& getTitle() const noexcept { return m_title; }
		
		[[nodiscard]] inline const IVec2& getPosition() const noexcept { return m_position; }

		[[nodiscard]] inline float getAspectRatio() const { return (float)m_size.x / (float)m_size.y; }

		[[nodiscard]] inline int getWidth() const noexcept { return m_size.x; }

		[[nodiscard]] inline int getHeight() const noexcept { return m_size.y; }

		[[nodiscard]] inline bool isVSyncEnabled() const noexcept { return m_isVSyncEnabled; }

		[[nodiscard]] inline bool isFullscreen() const noexcept { return m_isFullscreen; }

		[[nodiscard]] inline bool alwaysOnTop() const noexcept { return m_alwaysOnTop; }
	
		[[nodiscard]] inline bool shouldClose() const noexcept { return m_shouldClose; }

		// ==================== Operations ====================
		inline void toggleFullscreen() { setFullscreen(!m_isFullscreen); } // remove?

		bool setAspectRatio(float width, float height);

		bool setAlwaysOnTop(bool enable);

		bool setFullscreen(bool enable);

		bool setPosition(int x, int y);

		bool setVSync(bool enable);

		bool setTitle(const char* title);

		bool setIcon(Surface surface);

	private:
		// ==================== Interal Helpers ====================
		void handleResize(int width, int height);

		SDL_Window* m_window;
		std::string m_title;
		IVec2 m_position;
		IVec2 m_size;
		bool m_isVSyncEnabled;
		bool m_alwaysOnTop;
		bool m_isFullscreen;
		bool m_shouldClose;
	};
}