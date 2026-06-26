#pragma once
#include "engine/resources/resource_types.h"
#include "engine/rendering/render_api.h"

namespace cursed_engine
{
	// remake into a ServiceRegistry?
	struct EngineContext
	{
		struct PlatformServices
		{
			class InputHandler* inputHandler{};
			class FrameTimer* timer{};
		} platform;

		struct RenderingServices
		{
			RenderAPI rendererAPI;
			//class Renderer* renderer{};
		} rendering;

		struct AssetServices
		{
			class AssetManager* assetManager{};
			class Localization* localization{};
		} assets;

		struct ResourceServices
		{
			AudioManager* audioManager{};
			FontManager* fontManager{};
			TextureManager* textureManager{};
			class TextManager* textManager{};
			class TextFactory* textFactory{};
		} resources;

		struct ECSServices
		{
			class EntityFactory* entityFactory{};
			class ComponentRegistry* componentRegistry{};
			class SystemManager* systemManager{};
		} ecs;

		struct PhysicsServices
		{
			class Physics* physics{};
		} physics;

		struct AudioServices
		{
			class AudioController* audioController{};
		} audio;

		class ActionRegistry* actionRegistry;
		class EventBus* eventBus;
		class Settings* settings;
	};
}