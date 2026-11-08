#pragma once
#include "engine/platform/input_types.h"
#include "engine/math/vec2.hpp"
#include <array>

union SDL_Event;

namespace cursed_engine
{
	class EventBus;

	template <typename T>
	class Result;

	struct InputArgs
	{
		KeyBindings keyBindings;
		float mouseSensitivity = 1.f;
	};

	class InputHandler final
	{
	public:
		InputHandler(EventBus& eventBus);
		~InputHandler();

		InputHandler(const InputHandler&) = delete;
		InputHandler(InputHandler&&) = delete;

		InputHandler& operator=(const InputHandler&) = delete;
		InputHandler& operator=(InputHandler&&) = delete;

		Result<void> init(const InputArgs& args);
		void processInput(const SDL_Event& event);

		void beginFrame();
		void endFrame();

		// mark noexcept?
		[[nodiscard]] bool isKeyPressed(Key key) const;
		[[nodiscard]] bool isKeyReleased(Key key) const;
		[[nodiscard]] bool isKeyHeld(Key key) const;

		[[nodiscard]] bool isMouseBtnPressed(MouseButton button) const;
		[[nodiscard]] bool isMouseBtnReleased(MouseButton button) const;
		[[nodiscard]] bool isMouseBtnHeld(MouseButton button) const;

		[[nodiscard]] InputState getMouseInputState(MouseButton button) const noexcept;
		[[nodiscard]] InputState getKeyState(const InputInfo& info) const noexcept; // why acceot input info??

		[[nodiscard]] FVec2 getMousePosition() const;
		[[nodiscard]] FVec2 getMouseDelta() const;
		[[nodiscard]] float getMouseScroll() const;

	private:
		void handleMouseButtonEvent(const SDL_Event& event);
		void handleMouseMotionEvent(const SDL_Event& event);

		void handleMouseWheelEvent(const SDL_Event& event);
		void handleKeyEvent(const SDL_Event& event);

		struct MouseState
		{
			std::array<InputInfo, (std::size_t)MouseButton::Count> buttons;
			float x = 0.f;
			float y = 0.f;
			float scroll = 0.f;
		};

		using KeyInfo = std::array<InputInfo, (std::size_t)Key::COUNT>; // name KeyStates?

		KeyInfo m_keyInfo; // scan code(s) or key codes?
		MouseState m_mouseState;

		EventBus& m_eventBus; // base?
	};
}