#pragma once
#include "engine/core/subsystem.h"
#include "engine/math/vec2.h"
#include <array>
#include <SDL3/SDL.h>

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
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2,
		Count
	};

	struct MouseBtnPressedEvent // Shared events mouse btn and key?
	{
		MouseButton button;
	};

	struct MouseBtnReleasedEvent
	{
		MouseButton button;
	};

	struct KeyPressedEvent
	{
		SDL_Scancode key;
	};

	struct KeyReleasedEvent
	{
		SDL_Scancode key;
	};

	struct InputInfo
	{
		InputState inputState = InputState::None; // all three needed? or remove this?

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
		InputHandler(EventBus& eventBus);

		void init(const InputConfig& config); // copy move instead?
	
		void beginFrame();
		void processInput(const SDL_Event& event);
		void update();

		// Bind action function (action, key)
		// isAction triggered?
		// add noexcept....

		[[nodiscard]] bool isKeyPressed(SDL_Scancode code) const;

		[[nodiscard]] bool isKeyHeld(SDL_Scancode code) const;
		
		[[nodiscard]] bool isKeyReleased(SDL_Scancode code) const;

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
		using KeyInfo = std::array<InputInfo, SDL_Scancode::SDL_SCANCODE_COUNT>; // name KeyStates?

		KeyInfo m_keyInfo; // scan code(s) or key codes?
		MouseState m_mouseState;
		EventBus& m_eventBus;
	};
}