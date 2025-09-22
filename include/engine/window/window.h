#pragma once
#include "engine/window/window_config.h"
#include <filesystem>

typedef struct SDL_Window SDL_Window;
struct SDL_Surface;

union SDL_Event;

namespace cursed_engine
{
	struct WindowConfig;

	class Window
	{
	public:
		// ==================== Lifecycle ====================
		Window();
		~Window();

		void init(WindowConfig config);	// or rename: applyConfig();

		void shutdown(); // or destroy?

		// ==================== Event Handling ====================
		void processEvent(const SDL_Event& event);
		
		// ==================== Queries ====================
		[[nodiscard]] inline SDL_Window* getHandle() const noexcept { return m_window; }

		[[nodiscard]] inline int getWidth() const { return m_width; }

		[[nodiscard]] inline int getHeight() const { return m_height; }

		[[nodiscard]] inline float getAspectRatio() const { return (float)m_width / (float)m_height; }

		[[nodiscard]] inline bool isFullscreen() const noexcept { return m_isFullscreen; }

		[[nodiscard]] inline bool shouldClose() const noexcept { return m_shouldClose; }

		// ==================== Operations ====================
		inline void toggleFullscreen() { setFullscreen(!m_isFullscreen); }

		void setFullscreen(bool enable);

		void setTitle(const char* title);

		void setIcon(SDL_Surface* surface);

		void setIcon(const std::filesystem::path& path);

	private:
		// ==================== Interal Helpers ====================
		void handleResize(int width, int height);

		WindowConfig m_config;
		SDL_Window* m_window;
		int m_width; // store as vec2 later?
		int m_height;
		bool m_isFullscreen;
		bool m_shouldClose;
	};
}