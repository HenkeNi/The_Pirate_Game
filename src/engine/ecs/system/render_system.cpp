#include "engine/ecs/system/render_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/rendering/renderer.h"
#include "engine/resources/resource_manager.hpp"
#include "engine/rendering/texture.h"
#include "engine/rendering/render_types.h" // Put in renderer.h? to avoid including it

namespace cursed_engine
{
	RenderSystem::RenderSystem(Renderer& renderer, EngineResources& engineResources, AssetManager& assetManager)
		: m_renderer{ renderer }, m_engineResources{ engineResources }, m_assetManager{ assetManager }
	{
	}

	void RenderSystem::update(ECSRegistry& registry, float deltaTime)
	{
		// TODO; sort by handles? -> maybe sort using texture id (perhaps integer would be better than string?)

		m_renderer.clearScreen(); // HERE or in main loop?

		const auto componentView = registry.view<SpriteComponent, TransformComponent>();
		componentView.forEach([&](const SpriteComponent& spriteComponent, const TransformComponent& transformComponent)
			{

				const auto& textureAtlas = m_assetManager.getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

				const auto& textureHandle = m_engineResources.getHandle<Texture>(textureAtlas.textureID);
				auto& texture = m_engineResources.get<Texture>(textureHandle);

				// TODO; handle invalid/not loaded texture!

				const auto& pos = transformComponent.position;
				const auto& scale = transformComponent.scale;

				m_renderer.renderTexture(pos.x, pos.y, scale.x, scale.y, texture);
			});

		// ################### TEST #########################

#ifndef NDEBUG

		renderDebug(registry);

#endif

		auto textureHandleTest = m_engineResources.getHandle<Texture>("test3"); // pass in "coordinates"? or do that in texture?
		auto& testTexture = m_engineResources.get<Texture>(textureHandleTest);

		m_renderer.renderTexture(10, 10, 10, 10, testTexture);
		m_renderer.renderLine(0, 0, 100, 100, Color{ 123, 21, 32, 255 });

		m_renderer.present(); // Here or in main loop?
	}

	void RenderSystem::renderText(ECSRegistry& registry)
	{
		auto view = registry.view<TransformComponent, TextComponent>();
		
		// TODO; check if possible to have one argument const ref and one argument just ref...
		view.forEach([](const TransformComponent& transformComponent, TextComponent& textComponent)
			{
				// here or in text system?
				if (textComponent.isDirty)
				{
					// regenerateTexture...
					int y = 20;
				}

				int x = 20;
			});
	}

	void RenderSystem::renderDebug(ECSRegistry& registry)
	{
		// TODO; use RenderRects and batch the call, rather than passing each individual rects!

		// TODO; wrap in "limited" scope if multiple...
		auto view = registry.view<TransformComponent, BoundingBox>();
		view.forEach([&](TransformComponent& transformComponent, BoundingBox& boundingBox)
			{
				// pos 
				auto position = transformComponent.position;
				auto offset = boundingBox.offset;
				auto halfExtent = boundingBox.halfExtents;
				/*min.x += boundingBox.offset.x;
				min.y += boundingBox.offset.y;

				FVec2 max;
				max.x = min.x + boundingBox.halfExtents.x * 2;
				max.y = min.y + boundingBox.halfExtents.y * 2;
				*/
				FVec2 p1{ position.x + offset.x, position.y + offset.y };
				FVec2 p2{ p1.x, p1.y + halfExtent.y }; // halfextent is width?
				FVec2 p3{ p1.x + halfExtent.x, p1.y };
				FVec2 p4{ p3.x, p2.y };
				
				Color color{ 0, 0, 0, 0 };

				m_renderer.renderOutlineRect(FRect{ p1.x, p2.y, p3.x, p4.x }, Color::black);
				m_renderer.renderFillRect(FRect{ position.x, position.y, 100, 100 }, Color::red);

				/*m_renderer.renderLine(p1, p2, color);
				m_renderer.renderLine(p1, p3, color);
				m_renderer.renderLine(p2, p4, color);
				m_renderer.renderLine(p3, p4, color);*/
			});
	}
}