#pragma once

namespace cursed_engine
{
	struct InputConfig
	{

	};

	class InputHandler
	{
	public:
		void processInput();

		[[nodiscard]] bool isKeyPressed() const;
		[[nodiscard]] bool isKeyHeld() const;

	private:
		// registered key mappings? (config?)

	};
}