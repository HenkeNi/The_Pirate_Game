#include "engine/ecs/system/render_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/rendering/renderer.h"
#include "engine/resources/resource_manager.hpp"
#include "engine/rendering/texture.h"
#include "engine/rendering/render_types.h" // Put in renderer.h? to avoid including it

#include <source_location>
#include <iostream>
namespace cursed_engine
{
	RenderSystem::RenderSystem(Renderer& renderer, EngineResources& engineResources, AssetManager& assetManager)
		: m_renderer{ renderer }, m_engineResources{ engineResources }, m_assetManager{ assetManager }
	{
	}

	void RenderSystem::update(SystemContext& context)
	{
		// TODO; sort by handles? -> maybe sort using texture id (perhaps integer would be better than string?)

		m_renderer.clearScreen(); // HERE or in main loop?

		auto& registry = context.registry;
		auto& textureManager = m_engineResources.textureManager;

		const auto componentView = registry.view<SpriteComponent, TransformComponent>();
		componentView.forEach([&](const SpriteComponent& spriteComponent, const TransformComponent& transformComponent)
			{
				const auto& textureAtlas = m_assetManager.getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

				const auto& textureHandle = textureManager.getHandleById(textureAtlas.textureID);
				
				if (auto* texture = textureManager.get(textureHandle))
				{
					// TODO; handle invalid/not loaded texture!

					const auto& pos = transformComponent.position;
					const auto& scale = transformComponent.scale;

					m_renderer.renderTexture(pos.x, pos.y, scale.x, scale.y, *texture);
				}
			});

		// ################### TEST #########################

#ifndef NDEBUG

		renderDebug(registry);

#endif

		auto textureHandleTest = textureManager.getHandleById("test3"); // pass in "coordinates"? or do that in texture?
		
		if (auto* testTexture = textureManager.get(textureHandleTest))
		{
			m_renderer.renderTexture(10, 10, 10, 10, *testTexture);
		}
		
		m_renderer.renderLine(0, 0, 100, 100, Color{ 123, 21, 32, 255 });

		renderText(registry);
		//m_renderer.present(); // Here or in main loop?

		//renderText(registry); -> maybe not needed? text uses textuers...
	}

	void RenderSystem::renderText(ECSRegistry& registry)
	{
		auto view = registry.view<TransformComponent, TextComponent>();
		
		//Logger::logInfoExtended("HELLO");
		//Logger::logWarningExtended("Bye");
		//Logger::logErrorExtended("farewell");

		// TODO; check if possible to have one argument const ref and one argument just ref...
		view.forEach([&](const TransformComponent& transformComponent, TextComponent& textComponent)
			{
				auto textureHandle = textComponent.textureHandle;
				
				if (!textureHandle.isValid())
				{
					Logger::logError("[RenderSystem::renderText] - Invalid texture handle");
					//(false && "Invalid (text) texture handle"); // allow since text system generates new handle...

					return;
				}

				if (auto* texture = m_engineResources.textureManager.get(textureHandle))
				{
					const auto& position = transformComponent.position;
					const auto& size = transformComponent.scale; // this?
					m_renderer.renderTexture(position, size, *texture);
				}


				//m_renderer.renderTexture(position, 300.f, 100.f, texture);

				// here or in text system?
				//if (textComponent.isDirty)
				//{
				//	// regenerateTexture...
				//	int y = 20;
				//}

				// int x = 20;
			});
	}

	void RenderSystem::renderDebug(ECSRegistry& registry) // rename render colliders?
	{
		// TODO; use RenderRects and batch the call, rather than passing each individual rects!

		// TODO; wrap in "limited" scope if multiple...
		auto view = registry.view<TransformComponent, BoundingBox>();
		view.forEach([&](TransformComponent& transformComponent, BoundingBox& boundingBox)
			{
				// pos 
				//FVec2 min = transformComponent.position + boundingBox.offset;
				//FVec2 max = min + boundingBox.halfExtents;

				FVec2 position = transformComponent.position + boundingBox.offset;
				FVec2 size = boundingBox.halfSize * 2.f;
							
				m_renderer.renderOutlineRect(position.x, position.y, size.x, size.y, Color::red);
				//m_renderer.renderFillRect(FRect{ position.x, position.y, 100, 100 }, Color::red);
			});
	}
}