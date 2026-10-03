#pragma once
#include "engine/resources/texture/surface.h"
#include "engine/math/vec2.hpp"
#include <string>
#include <variant>

typedef struct SDL_Window SDL_Window;
union SDL_Event;

namespace cursed_engine
{
	// Consider; Add a window builder?
	// TODO; handle multiple vsync "modes"? - test / make sure, set vsync, etc works!
	// TODO; send event, resize, etc? TODO; set min/max aspect ratio?
	// Store minimized? etc? WindowState?

	struct WindowConfig;

	template <typename T>
	class Result;

	using Icon = std::variant<Surface>;

#pragma region Window

	class Window
	{
	public:
		struct Preferences
		{
			std::string title{};
			IVec2 position{};
			IVec2 size{};
			bool fullscreen{};
			bool vsync{};
			bool alwaysOnTop{};
			// bool resizable
		};

		struct State
		{
			bool minimized = false;
			bool focused = true;
			bool visible = true;
		};

		Window() = default;
		virtual ~Window() = default;

		Window(const Window&) = delete;
		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		// ==================== Lifecycle ====================
		Result<void> create(const WindowConfig& config);
		void destroy();

		// ==================== Getters ====================
		[[nodiscard]] inline const Preferences& getPreferences() const noexcept { return m_preferences; }
		[[nodiscard]] const State& getState() const noexcept { return m_state; }

		[[nodiscard]] virtual void* getNativeHandle() const noexcept = 0;

		// ==================== Setters ====================
		void setPreferences(Preferences preferences);
		bool setTitle(const char* title);

		bool setAlwaysOnTop(bool enable);
		bool setFullscreen(bool enable);

		bool setPosition(int x, int y);
		bool setSize(int width, int height);

		bool setIcon(const Icon& icon);
		bool setVSync(bool enable);

	protected:
		// ==================== Backend Hooks ====================
		virtual Result<void> onCreate(const WindowConfig& config) = 0;
		virtual void onDestroy() = 0;

		virtual bool onSetTitle(const char* title) = 0;
		virtual bool onSetAlwaysOnTop(bool enable) = 0;

		virtual bool onSetFullscreen(bool enable) = 0;
		virtual bool onSetPosition(int x, int y) = 0;

		virtual bool onSetSize(int width, int height) = 0;
		virtual bool onSetVSync(bool enable) = 0;
		virtual bool onSetIcon(const Icon& icon) = 0;

	private:
		Preferences m_preferences;
		State m_state;
	};

#pragma endregion

#pragma region SDL_Window

	class SDLWindow final : public Window
	{
	public:
		SDLWindow();
		SDLWindow(SDLWindow&& other) noexcept;
		SDLWindow& operator=(SDLWindow&& other) noexcept;

		// ==================== Event Processing ====================
		void processEvent(const SDL_Event& event);

		// ==================== Overrides ====================
		[[nodiscard]] void* getNativeHandle() const noexcept override;

	protected:
		Result<void> onCreate(const WindowConfig& config) override;
		void onDestroy() override;

		bool onSetTitle(const char* title) override;
		bool onSetAlwaysOnTop(bool enable) override;

		bool onSetFullscreen(bool enable) override;
		bool onSetPosition(int x, int y) override;

		bool onSetSize(int width, int height) override;
		bool onSetVSync(bool enable) override;
		bool onSetIcon(const Icon& icon) override;

	private:
		SDL_Window* m_window = nullptr;
	};

#pragma endregion
}