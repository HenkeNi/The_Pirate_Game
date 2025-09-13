#include "engine/input/input_handler.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <functional>
#include <string>
#include "engine/core/logger.h"

namespace cursed_engine
{
	void InputHandler::init(const InputConfig& config)
	{
		m_config = config;

		std::for_each(m_config.keyBindings.begin(), m_config.keyBindings.end(),
			[&](const auto& pair) { m_keyInfo[pair.first] = KeyInfo{ KeyState::None, false, false }; });
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
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			processMouseEvent(event);
			break;
		default:
			break;
		}
	}

	void InputHandler::update()
	{
		for (auto& [scancode, info] : m_keyInfo)
		{
			info.keyState = getKeyState(info);
			info.wasDown = info.isDown;
		}
	}

	bool InputHandler::isKeyPressed(SDL_Scancode code) const
	{
		// TODO; will crash if key not mapped!
		return m_keyInfo.at(code).keyState == KeyState::Pressed;
	}

	bool InputHandler::isKeyHeld(SDL_Scancode code) const
	{
		return m_keyInfo.at(code).keyState == KeyState::Held;
	}

	bool InputHandler::isKeyReleased(SDL_Scancode code) const
	{
		return m_keyInfo.at(code).keyState == KeyState::Released;
	}

	bool InputHandler::isMouseBtnPressed(uint8_t button) const
	{
		return false;
	}

	bool InputHandler::isMouseBtnHeld(uint8_t button) const
	{
		return false;
	}

	bool InputHandler::isMouseBtnReleased(uint8_t button) const
	{
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

	void InputHandler::processKeyEvent(const SDL_Event& event)
	{
		if (auto it = m_config.keyBindings.find(event.key.scancode); it != m_config.keyBindings.end())
		{
			bool isPressed = event.type == SDL_EVENT_KEY_DOWN;
			m_keyInfo[it->first].isDown = isPressed;
		}
	}

	void InputHandler::processMouseEvent(const SDL_Event& event)
	{
		float x, y;
		SDL_GetMouseState(&x, &y);

	}

	KeyState InputHandler::getKeyState(const KeyInfo& info) const noexcept
	{
		if (info.isDown && !info.wasDown) return KeyState::Pressed;
		if (info.isDown && info.wasDown) return KeyState::Held;
		if (!info.isDown && info.wasDown) return KeyState::Released;

		return KeyState::None;
	}
}