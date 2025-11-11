#pragma once
#include "engine/utils/non_copyable.h"

namespace cursed_engine
{ 
	struct AppContext
	{
		class SystemManager& systemManager;
		class InputHandler& inputHandler;
		class Renderer& renderer;
		class Window& window;
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

		virtual void onCreated(const AppContext& context) {};
		virtual void onDestroyed() {};
	};
}