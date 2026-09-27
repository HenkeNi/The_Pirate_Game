#include "engine/platform/input_api.h"
#include "engine/platform/input.h"

namespace cursed_engine
{
	InputAPI::InputAPI(Input* input)
		: m_input{ input }
	{
	}

	bool InputAPI::isKeyPressed(Key key) const
	{
		return m_input->isKeyPressed(key);
	}

	bool InputAPI::isKeyReleased(Key key) const
	{
		return m_input->isKeyReleased(key);
	}

	bool InputAPI::isKeyHeld(Key key) const
	{
		return m_input->isKeyHeld(key);
	}

	bool InputAPI::isMouseBtnPressed(MouseButton button) const
	{
		return m_input->isMouseBtnPressed(button);
	}

	bool InputAPI::isMouseBtnReleased(MouseButton button) const
	{
		return m_input->isMouseBtnReleased(button);
	}

	bool InputAPI::isMouseBtnHeld(MouseButton button) const
	{
		return m_input->isMouseBtnHeld(button);
	}

	InputState InputAPI::getMouseInputState(MouseButton button) const noexcept
	{ 
		return m_input->getMouseInputState(button);
	}

	InputState InputAPI::getKeyState(const InputInfo& info) const noexcept
	{
		return m_input->getKeyState(info);
	}

	FVec2 InputAPI::getMousePosition() const
	{
		return m_input->getMousePosition();
	}

	FVec2 InputAPI::getMouseDelta() const
	{
		return m_input->getMouseDelta();
	}

	float InputAPI::getMouseScroll() const
	{
		return m_input->getMouseScroll();
	}
}