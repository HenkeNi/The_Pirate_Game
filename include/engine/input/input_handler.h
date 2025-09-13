#pragma once
#include "input_config.h"

union SDL_Event;

namespace cursed_engine
{
	enum class KeyState
	{
		Pressed,
		Held,
		Released,
		None
	};

	struct KeyInfo
	{
		KeyState keyState = KeyState::None;

		bool isDown = false;
		bool wasDown = false;
	};

	struct MouseInfo
	{
		float x, y;
	};

	class InputHandler
	{
	public:
		void init(const InputConfig& config); // copy move instead?
		void processInput(const SDL_Event& event);
		void update();

		// Bind action function (action, key)

		[[nodiscard]] bool isKeyPressed(SDL_Scancode code) const;
		[[nodiscard]] bool isKeyHeld(SDL_Scancode code) const;
		[[nodiscard]] bool isKeyReleased(SDL_Scancode code) const;

		[[nodiscard]] bool isMouseBtnPressed(uint8_t button) const;
		[[nodiscard]] bool isMouseBtnHeld(uint8_t button) const;
		[[nodiscard]] bool isMouseBtnReleased(uint8_t button) const;

		[[nodiscard]] std::pair<float, float> getMousePosition() const;
		[[nodiscard]] std::pair<float, float> getMouseDelta() const;
		[[nodiscard]] float getMouseScroll() const;

	private:
		void processKeyEvent(const SDL_Event& event);
		void processMouseEvent(const SDL_Event& event);

		[[nodiscard]] KeyState getKeyState(const KeyInfo& info) const noexcept;

		// registered key mappings? (config?)
		//std::array<eKeyState, SDL_NUM_SCANCODES> m_keyStates; // store key state?
		InputConfig m_config;
		std::unordered_map<SDL_Scancode, KeyInfo> m_keyInfo; // scan code(s) or key codes?
	};
}