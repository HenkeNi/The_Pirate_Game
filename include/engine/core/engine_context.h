#pragma once
#include "engine/ecs/component/component_registry.h"
#include "engine/resources/resource_types.h"
#include "engine/rendering/render_api.h"
#include "engine/platform/input_api.h"

#include "engine/physics/physics.h" // put in api class?

namespace cursed_engine
{
	struct EngineContext
	{
		struct PlatformServices
		{
			InputAPI input{};
			class FrameTimer* timer{};
		} platform;

		struct RenderingServices
		{
			RenderAPI rendererAPI;
			class RenderPipeline& renderPipeline;
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
			//class TextFactory* textFactory{};
		} resources;

		struct ECSServices
		{
			class EntityFactory* entityFactory{};
			ComponentRegistry* componentRegistry{};
			class SystemManager* systemManager{};
		} ecs;

		struct PhysicsServices
		{
			PhysicsAPI physics{};
			class PhysicsDebugDraw* physicsDebugDraw{};
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