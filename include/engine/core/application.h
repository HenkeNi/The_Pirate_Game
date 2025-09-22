#pragma once
#include "engine/utils/non_copyable.h"

namespace cursed_engine
{
	struct EngineSystems 
	{
		class InputHandler& inputHandler;
		class Window& window;
		class Renderer& renderer;
		// event system;
		// audio audio;
		// frame timer?
	};

	class Application : private NonCopyable
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		virtual void onUpdate(float deltaTime) = 0;

		virtual void onCreated(const EngineSystems& systems) {};
		virtual void onDestroyed() {};
	};
}