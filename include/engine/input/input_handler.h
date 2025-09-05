#pragma once

union SDL_Event;

namespace cursed_engine
{
	struct InputConfig
	{
		// store key bindings..? (read from json)
	};

	class InputHandler
	{
	public:
		void processInput(SDL_Event* inputEvent);

		[[nodiscard]] bool isKeyPressed() const; // pass in key!
		[[nodiscard]] bool isKeyHeld() const;

	private:
		// registered key mappings? (config?)
		std::unordered_map<int, bool> m_keyStates; // store key state?
	};
}