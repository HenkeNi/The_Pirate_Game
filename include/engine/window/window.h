#pragma once
#include "engine/math/vec2.h"
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

		void init(const char* title, const WindowConfig& config);	// or rename: applyConfig();

		void shutdown(); // or destroy?

		// ==================== Event Handling ====================
		void processEvent(const SDL_Event& event);
		
		// ==================== Queries ====================
		[[nodiscard]] inline SDL_Window* getHandle() const noexcept { return m_window; }

		[[nodiscard]] inline int getWidth() const noexcept { return m_size.x; }

		[[nodiscard]] inline int getHeight() const noexcept { return m_size.y; }

		[[nodiscard]] inline float getAspectRatio() const { return (float)m_size.x / (float)m_size.y; }

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

		SDL_Window* m_window;
		IVec2 m_size;
		bool m_isFullscreen;
		bool m_shouldClose;
	};
}