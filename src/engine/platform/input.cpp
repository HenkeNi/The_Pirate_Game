#include "engine/platform/input.h"
#include "engine/core/settings/engine_config.h"
#include "engine/core/events/event_bus.h"
#include "engine/core/events/events.h"
#include "engine/core/logger.h"
#include "engine/core/Result.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <functional>
#include <string>

namespace cursed_engine
{
	SDLInput::SDLInput(EventBus& eventBus)
		: m_eventBus{ eventBus }
	{
	}

	Result<void> SDLInput::init(const InputConfig& config)
	{
		std::for_each(config.keyBindings.begin(), config.keyBindings.end(),
			[&](const auto& pair) { m_keyInfo[(std::size_t)pair.first] = InputInfo{ InputState::None, false, false }; });
	
		
		return Result<void>::success();
	}

	void SDLInput::beginFrame()
	{
		//for (auto& keyInfo : m_keyInfo)
		//{
		//	keyInfo.inputState = getKeyState(keyInfo);
		//	keyInfo.wasDown = keyInfo.isDown; // maybe dont`?
		//}

		for (auto& button : m_mouseState.buttons)
		{
			button.isDown = false; // correct??
			//button.wasDown = button.isDown;
		}
		
		m_mouseState.scroll = 0.f;
	}

	void SDLInput::processInput(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			handleKeyEvent(event);
			break;
		case SDL_EVENT_MOUSE_MOTION:
			handleMouseMotionEvent(event);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			handleMouseButtonEvent(event);
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			handleMouseWheelEvent(event);
			break;
		}

		//send input evnet...
		// TODO; send input event?!
	}

	void SDLInput::endFrame()
	{
		//for (auto& [scancode, info] : m_keyInfo)
		for (auto& keyInfo : m_keyInfo)
		{
			keyInfo.inputState = getKeyState(keyInfo);
			keyInfo.wasDown = keyInfo.isDown; // maybe dont`?
		}

		// here?
		for (auto& button : m_mouseState.buttons)
		{
			button.wasDown = button.isDown;
		}
	}

	bool SDLInput::isKeyPressed(Key key) const
	{
		//assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at((std::size_t)key).inputState == InputState::Pressed;
	}

	bool SDLInput::isKeyReleased(Key key) const
	{
		//assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at((std::size_t)key).inputState == InputState::Released;
	}

	bool SDLInput::isKeyHeld(Key key) const
	{
		//assert(m_keyInfo.contains(code) && "Key not registered in InputHandler!");
		return m_keyInfo.at((std::size_t)key).inputState == InputState::Held;
	}

	bool SDLInput::isMouseBtnPressed(MouseButton button) const
	{
		auto mouseState = SDL_GetMouseState(nullptr, nullptr);
	
		return mouseState & SDL_BUTTON_LMASK;

		//if (button != MouseButton::Count)
		//	return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Pressed;

		//return false;
	}

	bool SDLInput::isMouseBtnReleased(MouseButton button) const
	{
		if (button != MouseButton::Count)
			return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Released;

		return false;
	}

	bool SDLInput::isMouseBtnHeld(MouseButton button) const
	{
		if (button != MouseButton::Count)
			return m_mouseState.buttons[(std::size_t)button].inputState == InputState::Held;

		return false;
	}

	InputState SDLInput::getMouseInputState(MouseButton button) const noexcept
	{
		const auto& mouseButton = m_mouseState.buttons[(std::size_t)button]; // will crash if button not present!

		if (mouseButton.wasDown && mouseButton.isDown)
			return InputState::Held;
		else if (mouseButton.wasDown && !mouseButton.isDown)
			return InputState::Released;
		else if (!mouseButton.wasDown && mouseButton.isDown)
			return InputState::Pressed;

		return InputState::None;


		//return m_mouseState.buttons[(std::size_t)button].inputState;
	}

	InputState SDLInput::getKeyState(const InputInfo& info) const noexcept
	{
		if (info.isDown && !info.wasDown) return InputState::Pressed;
		if (info.isDown && info.wasDown) return InputState::Held;
		if (!info.isDown && info.wasDown) return InputState::Released;

		return InputState::None;
	}
	
	FVec2 SDLInput::getMousePosition() const
	{
		return FVec2{ m_mouseState.x, m_mouseState.y };
	}

	FVec2 SDLInput::getMouseDelta() const
	{
		assert(false && "Not implemented!");
		return FVec2{};
	}

	float SDLInput::getMouseScroll() const
	{
		return m_mouseState.scroll;
	}

	void SDLInput::handleMouseButtonEvent(const SDL_Event& event)
	{
		uint8_t button = event.button.button;
		auto& mouseButton = m_mouseState.buttons[button];

		//mouseButton.wasDown = mouseButton.wasDown;
		mouseButton.isDown = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
	}

	void SDLInput::handleMouseMotionEvent(const SDL_Event& event)
	{
		if (m_mouseState.x != event.motion.x || m_mouseState.y != event.motion.y)
		{
			m_mouseState.x = event.motion.x;
			m_mouseState.y = event.motion.y;

			// TOOD; send event
		}
	}

	void SDLInput::handleMouseWheelEvent(const SDL_Event& event)
	{
		m_mouseState.scroll = event.wheel.y;
	}

	void SDLInput::handleKeyEvent(const SDL_Event& event)
	{
		Key key = static_cast<Key>(event.key.scancode);
		auto& keyInfo = m_keyInfo[(std::size_t)key];

		//key.wasDown = key.isDown;
		keyInfo.isDown = (event.type == SDL_EVENT_KEY_DOWN);

		if (!keyInfo.wasDown && keyInfo.isDown)
			m_eventBus.publishInstantly<KeyPressedEvent>(key);
		else if (keyInfo.wasDown && !keyInfo.isDown)
			m_eventBus.publishInstantly<KeyReleasedEvent>(key);
	}
}