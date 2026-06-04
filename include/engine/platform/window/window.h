#pragma once
#include "engine/math/vec2.hpp"
#include <filesystem>

typedef struct SDL_Window SDL_Window;
union SDL_Event;

namespace cursed_engine
{
	struct WindowConfig;
	struct Surface;
	class Settings;
	class PlatformModule;

	//SDL_SetWindowPosition();
	//SDL_SetWindowAspectRatio();

	// window builder?

	class Window
	{
	public:
		// ==================== Lifecycle ====================
		Window();
		~Window();

		bool create(const char* title, const WindowConfig& config);	// or rename: applyConfig();
		void destroy();

		// ==================== Event Handling ====================
		void processEvent(const SDL_Event& event); // make private... // platformModule is friend class
		
		// ==================== Queries ====================
		[[nodiscard]] inline SDL_Window* getHandle() const noexcept { return m_window; } // rename? getInternal?

		[[nodiscard]] inline int getWidth() const noexcept { return m_size.x; }

		[[nodiscard]] inline int getHeight() const noexcept { return m_size.y; }

		[[nodiscard]] inline float getAspectRatio() const { return (float)m_size.x / (float)m_size.y; }

		[[nodiscard]] inline bool isFullscreen() const noexcept { return m_isFullscreen; }

		[[nodiscard]] inline bool shouldClose() const noexcept { return m_shouldClose; }

	private:
		friend class Settings;
		friend class PlatformModule; // remove? only allow icon and title to be set during init?

		// ==================== Operations ====================
		inline void toggleFullscreen() { setFullscreen(!m_isFullscreen); } // remove?

		void setFullscreen(bool enable);

		void setVSync(bool enable); // TEST!

		void setTitle(const char* title);

		void setIcon(Surface surface); // best way of passing it?

		// ==================== Interal Helpers ====================
		void handleResize(int width, int height);

		SDL_Window* m_window;
		IVec2 m_size;
		bool m_isFullscreen;
		bool m_shouldClose;

		bool m_isVSyncEnabled = false;
		// store bool is vsync is enabled? store vsync value? store defautl vsync value?
	};
}