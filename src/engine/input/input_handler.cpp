#include "engine/input/input_handler.h"
#include "engine/config/config_types.h"
#include "engine/core/logger.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <functional>
#include <string>

namespace cursed_engine
{
	void InputHandler::init(const InputConfig& config)
	{
		std::for_each(config.keyBindings.begin(), config.keyBindings.end(),
			[&](const auto& pair) { m_keyInfo[pair.first] = InputInfo{ InputState::None, false, false }; });
	}

	void InputHandler::processInput(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			processKeyEvent(event, event.type == SDL_EVENT_KEY_DOWN);
			break;
		case SDL_EVENT_MOUSE_MOTION:
			processMouseMotionEvent(event);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			processMouseButtonEvent(event, event.type == SDL_EVENT_KEY_DOWN);
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			processMouseWheelEvent(event);
			break;
		}
	}

	void InputHandler::update()
	{
		for (auto& [scancode, info] : m_keyInfo)
		{
			info.inputState = getKeyState(info);
			info.wasDown = info.isDown;
		}
	}

	bool InputHandler::isKeyPressed(SDL_Scancode code) const
	{
		assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at(code).inputState == InputState::Pressed;
	}

	bool InputHandler::isKeyHeld(SDL_Scancode code) const
	{
		assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at(code).inputState == InputState::Held;
	}

	bool InputHandler::isKeyReleased(SDL_Scancode code) const
	{
		assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at(code).inputState == InputState::Released;
	}

	bool InputHandler::isMouseBtnPressed(MouseButton button) const
	{
		if (button != MouseButton::Count)
			return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Pressed;

		return false;
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

	std::pair<float, float> InputHandler::getMousePosition() const
	{
		return std::pair<float, float>();
	}

	std::pair<float, float> InputHandler::getMouseDelta() const
	{
		return std::pair<float, float>();
	}

	float InputHandler::getMouseScroll() const
	{
		return 0.0f;
	}

	void InputHandler::processKeyEvent(const SDL_Event& event, bool isPressed)
	{
		// TODO;!!
		/*if (auto it = m_config.keyBindings.find(event.key.scancode); it != m_config.keyBindings.end())
		{
			m_keyInfo[it->first].isDown = isPressed;
		}*/
	}

	void InputHandler::processMouseButtonEvent(const SDL_Event& event, bool isPressed)
	{
		switch (event.button.button)
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
		}
	}

	void InputHandler::processMouseMotionEvent(const SDL_Event& event)
	{
		m_mouseState.x = event.motion.x;
		m_mouseState.y = event.motion.y;
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