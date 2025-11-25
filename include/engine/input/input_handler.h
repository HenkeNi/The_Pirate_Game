#pragma once
#include "engine/core/subsystem.h"
#include <array>
#include <SDL3/SDL.h>

union SDL_Event;


// [CONSIDER] able to call isActionPressed(Action)? -> or should input handler not know about actions?
// How to store registered keys? -> and save/update config

namespace cursed_engine
{
	struct InputConfig;

	enum class InputState
	{
		Pressed,
		Held,
		Released,
		None
	};

	enum class MouseButton : uint8_t 
	{
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2,
		Count
	};


	struct InputInfo
	{
		InputState inputState = InputState::None;

		bool isDown = false;
		bool wasDown = false;
	};

	struct MouseState
	{
		std::array<InputInfo, (std::size_t)MouseButton::Count> buttons;
		float x = 0.f;
		float y = 0.f;
		float scroll = 0.f;
	};

	class InputHandler : public Subsystem
	{
	public:
		void init(const InputConfig& config); // copy move instead?
		void processInput(const SDL_Event& event);
		void update();

		// Bind action function (action, key)
		// isAction triggered?

		[[nodiscard]] bool isKeyPressed(SDL_Scancode code) const;

		[[nodiscard]] bool isKeyHeld(SDL_Scancode code) const;
		
		[[nodiscard]] bool isKeyReleased(SDL_Scancode code) const;

		[[nodiscard]] bool isMouseBtnPressed(MouseButton button) const;
		
		[[nodiscard]] bool isMouseBtnHeld(MouseButton button) const;
		
		[[nodiscard]] bool isMouseBtnReleased(MouseButton button) const;

		[[nodiscard]] std::pair<float, float> getMousePosition() const;
		
		[[nodiscard]] std::pair<float, float> getMouseDelta() const;
		
		[[nodiscard]] float getMouseScroll() const;

	private:
		void processKeyEvent(const SDL_Event& event, bool isPressed);

		void processMouseButtonEvent(const SDL_Event& event, bool isPressed);

		void processMouseMotionEvent(const SDL_Event& event);

		void processMouseWheelEvent(const SDL_Event& event);

		[[nodiscard]] InputState getKeyState(const InputInfo& info) const noexcept;

		// registered key mappings? (config?)
		//std::array<eKeyState, SDL_NUM_SCANCODES> m_keyStates; // store key state?

		std::unordered_map<SDL_Scancode, InputInfo> m_keyInfo; // scan code(s) or key codes?
		MouseState m_mouseState;
	};
}