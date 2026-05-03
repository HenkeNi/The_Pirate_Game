#include "engine/input/input_handler.h"
//#include "engine/config/config_types.h"
#include "engine/config/config_manager.h"
#include "engine/core/logger.h"
#include "engine/events/event_bus.h"
#include "engine/events/events.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <functional>
#include <string>

namespace cursed_engine
{
	InputHandler::InputHandler(EventBus& eventBus)
		: m_eventBus{ eventBus }
	{
	}

	void InputHandler::init(const InputConfig& config)
	{
		std::for_each(config.keyBindings.begin(), config.keyBindings.end(),
			[&](const auto& pair) { m_keyInfo[pair.first] = InputInfo{ InputState::None, false, false }; });
	}

	void InputHandler::beginFrame()
	{
		for (auto& keyInfo : m_keyInfo)
		{
			keyInfo.inputState = getKeyState(keyInfo);
			keyInfo.wasDown = keyInfo.isDown; // maybe dont`?
		}

		for (auto& button : m_mouseState.buttons)
		{
			button.wasDown = button.isDown;
		}
		
		m_mouseState.scroll = 0.f;
	}

	void InputHandler::processInput(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			processKeyEvent(event);
			break;
		case SDL_EVENT_MOUSE_MOTION:
			processMouseMotionEvent(event);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			processMouseButtonEvent(event);
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			processMouseWheelEvent(event);
			break;
		}

		//send input evnet...
		// TODO; send input event?!
	}

	void InputHandler::update()
	{
		//for (auto& [scancode, info] : m_keyInfo)
		for (auto& keyInfo : m_keyInfo)
		{
			keyInfo.inputState = getKeyState(keyInfo);
			keyInfo.wasDown = keyInfo.isDown; // maybe dont`?
		}
	}

	bool InputHandler::isKeyPressed(SDL_Scancode code) const
	{
		//assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at(code).inputState == InputState::Pressed;
	}

	bool InputHandler::isKeyHeld(SDL_Scancode code) const
	{
		//assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at(code).inputState == InputState::Held;
	}

	bool InputHandler::isKeyReleased(SDL_Scancode code) const
	{
		//assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at(code).inputState == InputState::Released;
	}

	bool InputHandler::isMouseBtnPressed(MouseButton button) const
	{
		auto mouseState = SDL_GetMouseState(nullptr, nullptr);
	
		return mouseState & SDL_BUTTON_LMASK;

		//if (button != MouseButton::Count)
		//	return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Pressed;

		//return false;
	}

	bool InputHandler::isMouseBtnHeld(MouseButton button) const
	{
		if (button != MouseButton::Count)
			return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Held;

		return false;
	}

	bool InputHandler::isMouseBtnReleased(MouseButton button) const
	{
		if (button != MouseButton::Count)
			return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Released;

		return false;
	}

	InputState InputHandler::getMouseInputState(MouseButton button) const
	{
		const auto& mouseButton = m_mouseState.buttons[(std::size_t)button];

		if (mouseButton.wasDown && mouseButton.isDown)
			return InputState::Held;
		else if (mouseButton.wasDown && !mouseButton.isDown)
			return InputState::Released;
		else if (!mouseButton.wasDown && mouseButton.isDown)
			return InputState::Pressed;

		return InputState::None;


		//return m_mouseState.buttons[(std::size_t)button].inputState;
	}

	FVec2 InputHandler::getMousePosition() const
	{
		return FVec2{ m_mouseState.x, m_mouseState.y };
	}

	FVec2 InputHandler::getMouseDelta() const
	{
		assert(false && "Not implemented!");
		return FVec2{};
	}

	float InputHandler::getMouseScroll() const
	{
		return m_mouseState.scroll;
	}

	void InputHandler::processKeyEvent(const SDL_Event& event)
	{
		SDL_Scancode keyCode = event.key.scancode;
		auto& key = m_keyInfo[keyCode];

		//key.wasDown = key.isDown;
		key.isDown = (event.type == SDL_EVENT_KEY_DOWN);

		if (!key.wasDown && key.isDown)
			m_eventBus.publishInstantly<KeyPressedEvent>(keyCode);
		else if (key.wasDown && !key.isDown)
			m_eventBus.publishInstantly<KeyReleasedEvent>(keyCode);
	}

	void InputHandler::processMouseButtonEvent(const SDL_Event& event)
	{
		uint8_t button = event.button.button;
		auto& mouseButton = m_mouseState.buttons[button];

		mouseButton.wasDown = mouseButton.wasDown;
		mouseButton.isDown = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);

		//auto previousState = mouseButton.currentState;

		//bool isDown = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;

		//if (previousState == InputState::Held && isDown)
		//{
		//	// held
		//}
		//else if (previousState == InputState::None && isDown)
		//{
		//	// pressed
		//}



		//mouseButton.previousState = mouseButton.currentState;



		//bool isDown = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;

		//if (isDown)
		//{
		//	//mouseButton.inputState = InputState::Pressed;
		//	//mouseButton.inputState =
		//}


		//mouseButton.wasDown = mouseButton.isDown;
		//mouseButton.isDown = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);

		//if (!mouseButton.wasDown && mouseButton.isDown)
		//	m_eventBus.publishInstantly<MouseBtnPressedEvent>((MouseButton)button);
		//else if (mouseButton.wasDown && !mouseButton.isDown)
		//	m_eventBus.publishInstantly<MouseBtnReleasedEvent>((MouseButton)button);

		/*switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			m_mouseState.buttons[(std::size_t)MouseButton::Left].isDown = isPressed;
			break;
		case SDL_BUTTON_RIGHT:
			m_mouseState.buttons[(std::size_t)MouseButton::Right].isDown = isPressed;
			break;
		case SDL_BUTTON_MIDDLE:
			m_mouseState.buttons[(std::size_t)MouseButton::Middle].isDown = isPressed;
			break;
		}*/
	}

	void InputHandler::processMouseMotionEvent(const SDL_Event& event)
	{
		if (m_mouseState.x != event.motion.x || m_mouseState.y != event.motion.y)
		{
			m_mouseState.x = event.motion.x;
			m_mouseState.y = event.motion.y;

			// TOOD; send event
		}
	}

	void InputHandler::processMouseWheelEvent(const SDL_Event& event)
	{
		m_mouseState.scroll = event.wheel.y;
	}

	InputState InputHandler::getKeyState(const InputInfo& info) const noexcept
	{
		if (info.isDown && !info.wasDown) return InputState::Pressed;
		if (info.isDown && info.wasDown) return InputState::Held;
		if (!info.isDown && info.wasDown) return InputState::Released;

		return InputState::None;
	}
}