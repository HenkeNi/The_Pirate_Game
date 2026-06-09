#pragma once
#include "engine/utils/non_copyable.h"
#include "engine/math/vec2.hpp"

typedef struct SDL_Window SDL_Window;
union SDL_Event;

namespace cursed_engine
{
	// TODO; Add a window builder?
	// TODO; handle multiple vsync "modes"?

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

		[[nodiscard]] inline float getAspectRatio() const { return (float)m_size.x / (float)m_size.y; }

		[[nodiscard]] inline int getWidth() const noexcept { return m_size.x; }

		[[nodiscard]] inline int getHeight() const noexcept { return m_size.y; }

		[[nodiscard]] inline bool isFullscreen() const noexcept { return m_isFullscreen; }

		[[nodiscard]] inline bool shouldClose() const noexcept { return m_shouldClose; }

		// ==================== Operations ====================
		inline void toggleFullscreen() { setFullscreen(!m_isFullscreen); } // remove?

		void setAspectRatio(float width, float height);

		void setAlwaysOnTop(bool enable);

		void setFullscreen(bool enable);

		void setPosition(float x, float y);

		void setVSync(bool enable); // TEST!

		void setTitle(const char* title);

		void setIcon(Surface surface);

	private:
		// ==================== Interal Helpers ====================
		void handleResize(int width, int height);

		SDL_Window* m_window;
		IVec2 m_size;
		bool m_isVSyncEnabled;
		bool m_isFullscreen;
		bool m_shouldClose;
	};
}