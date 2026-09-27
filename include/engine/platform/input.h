#pragma once
#include "engine/math/vec2.hpp"
#include <array>

union SDL_Event;

// [CONSIDER] able to call isActionPressed(Action)? -> or should input handler not know about actions?
// How to store registered keys? -> and save/update config

namespace cursed_engine
{
	class EventBus;
	struct InputConfig;
	
	template <typename T>
	class Result;
	
	// TODO; put in input_types or platform_types??

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

	// Input API? (facade)

#pragma region Input

	// InputHandler?

	class Input
	{
	public:
		virtual ~Input() = default;

		// mark noexcept?
		[[nodiscard]] virtual bool isKeyPressed(Key key) const = 0;
		[[nodiscard]] virtual bool isKeyReleased(Key key) const = 0;
		[[nodiscard]] virtual bool isKeyHeld(Key key) const = 0;

		[[nodiscard]] virtual bool isMouseBtnPressed(MouseButton button) const = 0;
		[[nodiscard]] virtual bool isMouseBtnReleased(MouseButton button) const = 0;
		[[nodiscard]] virtual bool isMouseBtnHeld(MouseButton button) const = 0;
	
		[[nodiscard]] virtual InputState getMouseInputState(MouseButton button) const noexcept = 0;
		[[nodiscard]] virtual InputState getKeyState(const InputInfo& info) const noexcept = 0;

		[[nodiscard]] virtual FVec2 getMousePosition() const = 0;
		[[nodiscard]] virtual FVec2 getMouseDelta() const = 0;
		[[nodiscard]] virtual float getMouseScroll() const = 0;
	};

#pragma endregion

#pragma region SDL_Input

	class SDLInput : public Input
	{
	public:
		SDLInput(EventBus& eventBus);
		
		Result<void> init(const InputConfig& config); // virtual?
		void processInput(const SDL_Event& event); // virtual? or SDLPlatform knows about SDLInput so maybe fine?

		void beginFrame();
		void endFrame();

		// Bind action function (action, key)
		// isAction triggered?

		[[nodiscard]] bool isKeyPressed(Key key) const override;
		[[nodiscard]] bool isKeyReleased(Key key) const override;
		[[nodiscard]] bool isKeyHeld(Key key) const override;

		[[nodiscard]] bool isMouseBtnPressed(MouseButton button) const override;
		[[nodiscard]] bool isMouseBtnReleased(MouseButton button) const override;
		[[nodiscard]] bool isMouseBtnHeld(MouseButton button) const override;
	
		// REMOVE?? 
		[[nodiscard]] InputState getMouseInputState(MouseButton button) const noexcept override; // rename input state?
		[[nodiscard]] InputState getKeyState(const InputInfo& info) const noexcept override;

		[[nodiscard]] FVec2 getMousePosition() const override;
		[[nodiscard]] FVec2 getMouseDelta() const override;
		[[nodiscard]] float getMouseScroll() const override;

	private:
		void handleMouseButtonEvent(const SDL_Event& event);
		void handleMouseMotionEvent(const SDL_Event& event);
		void handleMouseWheelEvent(const SDL_Event& event);
		void handleKeyEvent(const SDL_Event& event);
		// registered key mappings? (config?)
		//std::array<eKeyState, SDL_NUM_SCANCODES> m_keyStates; // store key state?

		//using KeyInfo = std::unordered_map<SDL_Scancode, InputInfo>; // name KeyStates?
		using KeyInfo = std::array<InputInfo, (std::size_t)Key::COUNT>; // name KeyStates?

		KeyInfo m_keyInfo; // scan code(s) or key codes?
		MouseState m_mouseState;

		EventBus& m_eventBus; // base?
	};

#pragma endregion
}