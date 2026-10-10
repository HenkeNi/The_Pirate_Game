#pragma once
#include "engine/math/vec2.hpp"

namespace cursed_engine
{
	class InputHandler;
	struct MouseState;
	struct InputInfo;
	enum class InputState;
	enum class Key;
	enum class MouseButton : uint8_t;

	class InputAPI
	{
	public:
		InputAPI(InputHandler* inputHandler = nullptr);

		[[nodiscard]] bool isKeyPressed(Key key) const;
		[[nodiscard]] bool isKeyReleased(Key key) const;
		[[nodiscard]] bool isKeyHeld(Key key) const;

		[[nodiscard]] bool isMouseBtnPressed(MouseButton button) const;
		[[nodiscard]] bool isMouseBtnReleased(MouseButton button) const;
		[[nodiscard]] bool isMouseBtnHeld(MouseButton button) const;

		[[nodiscard]] InputState getMouseInputState(MouseButton button) const noexcept;
		[[nodiscard]] InputState getKeyState(const InputInfo& info) const noexcept;

		[[nodiscard]] FVec2 getMousePosition() const;
		[[nodiscard]] FVec2 getMouseDelta() const;
		[[nodiscard]] float getMouseScroll() const;

	private:
		InputHandler* m_inputHandler;
	};
}