#pragma once
#include "engine/math/vec2.hpp"
#include <array>
//#include <SDL3/SDL.h>

union SDL_Event;

// TODO; hide sdl_scancode...
// [CONSIDER] able to call isActionPressed(Action)? -> or should input handler not know about actions?
// How to store registered keys? -> and save/update config

namespace cursed_engine
{
	class EventBus;
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
		Left = 1,
		Middle = 2,
		Right = 3,
		X1 = 4,
		X2 = 5,
		Count
	};

	enum class Key
	{
		UNKNOWN = 0,
		A = 4,
		B = 5,
		C = 6,
		D = 7,
		E = 8,
		F = 9,
		G = 10,
		H = 11,
		I = 12,
		J = 13,
		K = 14,
		L = 15,
		M = 16,
		N = 17,
		O = 18,
		P = 19,
		Q = 20,
		R = 21,
		S = 22,
		T = 23,
		U = 24,
		V = 25,
		W = 26,
		X = 27,
		Y = 28,
		Z = 29,

		Num1 = 30,
		Num2 = 31,
		Num3 = 32,
		Num4 = 33,
		Num5 = 34,
		Num6 = 35,
		Num7 = 36,
		Num8 = 37,
		Num9 = 38,
		Num0 = 39,

		RETURN = 40,
		ESCAPE = 41,
		BACKSPACE = 42,
		TAB = 43,
		SPACE = 44,

		MINUS = 45,
		EQUALS = 46,
		LEFTBRACKET = 47,
		RIGHTBRACKET = 48,
		BACKSLASH = 49,
		SEMICOLON = 51,
		APOSTROPHE = 52,
		GRAVE = 53,
		COMMA = 54,
		PERIOD = 55,
		SLASH = 56,
		COUNT = 512
	};

	struct InputInfo
	{
		InputState inputState = InputState::None; // Union? or remove?
		//InputState currentState = InputState::None; // all three needed? or remove this?
		//InputState previousState = InputState::None;
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

	class InputHandler
	{
	public:
		InputHandler(EventBus& eventBus);

		void init(const InputConfig& config); // copy move instead?

		void beginFrame();
		void processInput(const SDL_Event& event);
		void endFrame();

		// Bind action function (action, key)
		// isAction triggered?
		// add noexcept....

		[[nodiscard]] bool isKeyPressed(Key key) const;

		[[nodiscard]] bool isKeyHeld(Key key) const;

		[[nodiscard]] bool isKeyReleased(Key key) const;

		[[nodiscard]] bool isMouseBtnPressed(MouseButton button) const;

		[[nodiscard]] bool isMouseBtnHeld(MouseButton button) const;

		[[nodiscard]] bool isMouseBtnReleased(MouseButton button) const;

		// REMOVE?? 
		[[nodiscard]] InputState getMouseInputState(MouseButton button) const; // rename input state?

		[[nodiscard]] FVec2 getMousePosition() const;

		[[nodiscard]] FVec2 getMouseDelta() const;

		[[nodiscard]] float getMouseScroll() const;

	private:
		void processKeyEvent(const SDL_Event& event);

		void processMouseButtonEvent(const SDL_Event& event);

		void processMouseMotionEvent(const SDL_Event& event);

		void processMouseWheelEvent(const SDL_Event& event);

		[[nodiscard]] InputState getKeyState(const InputInfo& info) const noexcept;

		// registered key mappings? (config?)
		//std::array<eKeyState, SDL_NUM_SCANCODES> m_keyStates; // store key state?

		//using KeyInfo = std::unordered_map<SDL_Scancode, InputInfo>; // name KeyStates?
		using KeyInfo = std::array<InputInfo, (std::size_t)Key::COUNT>; // name KeyStates?

		KeyInfo m_keyInfo; // scan code(s) or key codes?
		MouseState m_mouseState;
		EventBus& m_eventBus;
	};
}