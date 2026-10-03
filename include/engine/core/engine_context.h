#pragma once
#include "engine/ecs/component/component_registry.h"
#include "engine/resources/audio/audio_manager.h"
#include "engine/resources/font/font_manager.h"
#include "engine/resources/texture/texture_manager.h"
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
			const class TextCreator* textCreator{};
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