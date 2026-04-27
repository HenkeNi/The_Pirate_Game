#pragma once
#include "engine/core/subsystem.h"
#include "engine/math/vec2.h"
#include <filesystem>

typedef struct SDL_Window SDL_Window;
union SDL_Event;

namespace cursed_engine
{
	struct WindowConfig;
	struct Surface;

	// window builder?

	class Window : public Subsystem
	{
	public:
		// ==================== Lifecycle ====================
		Window();
		~Window();

		void create(const char* title, const WindowConfig& config);	// or rename: applyConfig();
		void shutdown() override; // or destroy? or both?

		// ==================== Event Handling ====================
		void processEvent(const SDL_Event& event);
		
		// ==================== Queries ====================
		[[nodiscard]] inline SDL_Window* getHandle() const noexcept { return m_window; } // rename?

		[[nodiscard]] inline int getWidth() const noexcept { return m_size.x; }

		[[nodiscard]] inline int getHeight() const noexcept { return m_size.y; }

		[[nodiscard]] inline float getAspectRatio() const { return (float)m_size.x / (float)m_size.y; }

		[[nodiscard]] inline bool isFullscreen() const noexcept { return m_isFullscreen; }

		[[nodiscard]] inline bool shouldClose() const noexcept { return m_shouldClose; }

		// ==================== Operations ====================
		inline void toggleFullscreen() { setFullscreen(!m_isFullscreen); }

		void setFullscreen(bool enable);

		void setTitle(const char* title);

		void setIcon(Surface surface); // best way of passing it?
		
	private:
		// ==================== Interal Helpers ====================
		void handleResize(int width, int height);

		SDL_Window* m_window;
		IVec2 m_size;
		bool m_isFullscreen;
		bool m_shouldClose;
	};
}