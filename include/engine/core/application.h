#pragma once
#include "engine/utils/non_copyable.h"
#include <filesystem>

#include "engine/core/registry_aliases.h"

namespace cursed_engine
{ 
	template <typename T, typename ID>
	class TypeRegistry;

	struct AppContext
	{
		class SystemManager& systemManager;
		class InputHandler& inputHandler;
		class AssetManager& assetManager;
		class EntityFactory& entityFactory;
		class Renderer& renderer;
		class Window& window;
		ComponentRegistry& componentData;
		const std::filesystem::path& assetRoot;
		
		// ecs registry?
		// entity factory? or store EntityFactory as protected in Application class?

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