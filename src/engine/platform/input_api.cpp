#include "engine/platform/input_api.h"
#include "engine/platform/input_handler.h"

namespace cursed_engine
{
	InputAPI::InputAPI(InputHandler* inputHandler)
		: m_inputHandler{ inputHandler }
	{
	}

	bool InputAPI::isKeyPressed(Key key) const
	{
		return m_inputHandler->isKeyPressed(key);
	}

	bool InputAPI::isKeyReleased(Key key) const
	{
		return m_inputHandler->isKeyReleased(key);
	}

	bool InputAPI::isKeyHeld(Key key) const
	{
		return m_inputHandler->isKeyHeld(key);
	}

	bool InputAPI::isMouseBtnPressed(MouseButton button) const
	{
		return m_inputHandler->isMouseBtnPressed(button);
	}

	bool InputAPI::isMouseBtnReleased(MouseButton button) const
	{
		return m_inputHandler->isMouseBtnReleased(button);
	}

	bool InputAPI::isMouseBtnHeld(MouseButton button) const
	{
		return m_inputHandler->isMouseBtnHeld(button);
	}

	InputState InputAPI::getMouseInputState(MouseButton button) const noexcept
	{ 
		return m_inputHandler->getMouseInputState(button);
	}

	InputState InputAPI::getKeyState(const InputInfo& info) const noexcept
	{
		return m_inputHandler->getKeyState(info);
	}

	FVec2 InputAPI::getMousePosition() const
	{
		return m_inputHandler->getMousePosition();
	}

	FVec2 InputAPI::getMouseDelta() const
	{
		return m_inputHandler->getMouseDelta();
	}

	float InputAPI::getMouseScroll() const
	{
		return m_inputHandler->getMouseScroll();
	}
}