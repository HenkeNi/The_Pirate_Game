#include "engine/ecs/system/world_render_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/rendering/render_types.h"

#include "engine/math/vec2.hpp"

#include "engine/physics/physics_debug_draw.h"

namespace cursed_engine
{
#pragma region Helper

	FVec2 computeDrawPosition(const FVec2& position, const FVec2& size, const FVec2& pivot) 
	{
		return position - FVec2{ pivot.x * size.x, pivot.y * size.y };
	}

#pragma endregion

	WorldRenderSystem::WorldRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer, PhysicsDebugDraw* physicsDebugDraw)
		: m_textureManager{ std::move(textureManager) }, m_assetManager{ assetManager }, m_renderer{ std::move(renderer) }, m_physicsDebugDraw{ physicsDebugDraw }
	{
		// Reserve appropriate size?
		m_entityBuffer.reserve(1000);
	}

	void WorldRenderSystem::update(SystemContext& context)
	{
		ECSRegistry& registry = context.registry;

		std::optional<Entity> cameraEntity = registry.view<CameraComponent>()
			.findFirst([](const CameraComponent& cameraComponent)
				{
					return cameraComponent.isActive;
				});

		if (!cameraEntity)
		{
			//Logger::logWarning("[WorldRenderSystem::Update] - No active camera found! Skipping world rendering");
			return;
		}

		Entity camera = cameraEntity.value();

		const auto& transformComponent = registry.getComponent<TransformComponent>(camera);
		const auto& cameraComponent = registry.getComponent<CameraComponent>(camera);

		// IS PIVOT CORRECT HERE??
		// TODO; use viewport size instead?
		m_renderer.setRenderState(RenderState{ 
			View
			{ 
				//transformComponent.position + transformComponent.pivot, 
				transformComponent.position, 
				transformComponent.rotation, 
				cameraComponent.zoom 
			}, 
			Projection
			{ 
				cameraComponent.viewportSize, 
				FVec2{ 0.f, 0.f } } 
			}
		); // view, projection				

		// Rect camera view?
		FAABB viewBounds{
			FVec2{
				transformComponent.position.x,
				transformComponent.position.y
			},
			FVec2 {
				transformComponent.position.x + cameraComponent.viewportSize.x,
				transformComponent.position.y + cameraComponent.viewportSize.y
			}
		};
		//FAABB viewBounds{
		//	FVec2{
		//		transformComponent.position.x + transformComponent.pivot.x,
		//		transformComponent.position.y + transformComponent.pivot.y
		//	},
		//	FVec2 {
		//		transformComponent.position.x + transformComponent.pivot.x + cameraComponent.viewportSize.x,
		//		transformComponent.position.y + transformComponent.pivot.y + cameraComponent.viewportSize.y
		//	}
		//};

		renderSprites(registry, viewBounds);
		renderDebug(registry, viewBounds);

		if (m_physicsDebugDraw)
			m_physicsDebugDraw->draw();
	}

	void WorldRenderSystem::renderSprites(ECSRegistry& registry, const FAABB& viewBounds)
	{
		// Filter out ui component entities
		m_entityBuffer.clear();

		auto view = registry.view<SpriteComponent, TransformComponent>();
		view.exclude<UIComponent>();

		// TODO; make so for Each works with just an entity as argument? -> check how it's done in Entt...
		view.forEach([&](Entity entity, const SpriteComponent& spriteComponent, const TransformComponent& transformComponent) // fix so works with only entity!
			{
				const AtlasRegion& region = spriteComponent.atlasRegion;
				const FAABB spriteBounds{ transformComponent.position, FVec2{ transformComponent.position.x + region.rect.w, transformComponent.position.y + region.rect.h } };

				if (!viewBounds.intersects(spriteBounds))
					return;

				m_entityBuffer.push_back(std::move(entity)); // Move redundant here?
			});


		std::ranges::sort(m_entityBuffer, [&](Entity lhs, Entity rhs)
			{
				return registry.getComponent<TransformComponent>(lhs).position.y < registry.getComponent<TransformComponent>(rhs).position.y;
				//return registry.getComponent<SpriteComponent>(lhs).zIndex < registry.getComponent<SpriteComponent>(rhs).zIndex;
			});

		for (const auto& entity : m_entityBuffer)
		{
			const auto& [spriteComponent, transformComponent] = registry.getComponents<SpriteComponent, TransformComponent>(entity);

			const AtlasRegion& region = spriteComponent.atlasRegion;

			//const FAABB spriteBounds{ transformComponent.position, FVec2{ transformComponent.position.x + region.rect.w, transformComponent.position.y + region.rect.h } };

			//if (!viewBounds.intersects(spriteBounds))
			//	return;

			const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

			const auto& textureHandle = m_textureManager->getHandleById(textureAtlas.textureId);

			if (auto* texture = m_textureManager->get(textureHandle))
			{
				FVec2 position = transformComponent.position;

				const FVec2 scaledSize = region.getSize() * transformComponent.scale;
				
				position -= region.pivot * scaledSize;

				//position -= scale * region.pivot;
				//position = computeDrawPosition(position, , region.pivot); // HOTPATH? Dont call compute? dont construct FVec for size?

				FRect src = (FRect)region.rect;
				FRect dst{ position.x, position.y, scaledSize.x, scaledSize.y };

				m_renderer.drawTexture(*texture, std::move(src), std::move(dst), spriteComponent.color);
			}
		}

		// Sort entities after texture atlas id? or handle?
		//view.forEach([&](const SpriteComponent& spriteComponent, const TransformComponent& transformComponent) // TODO; handle filtering... (UIComponent not used)
		//	{
		//		//const FAABB spriteBounds{ transformComponent.position, transformComponent.position + transformComponent.scale } }; // TODO; dont use scale!!?

		//		const AtlasRegion& region = spriteComponent.atlasRegion;

		//		const FAABB spriteBounds{ transformComponent.position,
		//		FVec2{ transformComponent.position.x + region.rect.w,
		//			transformComponent.position.y + region.rect.h } };

		//		if (!viewBounds.intersects(spriteBounds))
		//			return;

		//		const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

		//		const auto& textureHandle = m_textureManager->getHandleById(textureAtlas.textureId);

		//		if (auto* texture = m_textureManager->get(textureHandle))
		//		{
		//			FVec2 position = transformComponent.position;
		//			const FVec2& scale = transformComponent.scale;

		//			position -= scale * transformComponent.pivot;

		//			FRect src = (FRect)region.rect;
		//			FRect dst{ position.x, position.y, scale.x, scale.y };

		//			m_renderer.drawTexture(*texture, std::move(src), std::move(dst), spriteComponent.color);
		//		}
		//	});
	}

	void WorldRenderSystem::renderDebug(ECSRegistry& registry, const FAABB& viewBounds)
	{
		/*auto view = registry.view<TransformComponent, BoundingBoxComponent, UIComponent>();
		view.forEach([&](Entity entity, TransformComponent& transformComponent, BoundingBoxComponent& boundingBoxComponent, const UIComponent&)
			{
				Color color = Color::green;

				if (auto* buttonComponent = registry.tryGetComponent<ButtonComponent>(entity))
				{
					switch (buttonComponent->currentState)
					{
					case ButtonComponent::State::Normal:
						color = Color::green;
						break;

					case ButtonComponent::State::Hovered:
						color = Color::yellow;
						break;

					case ButtonComponent::State::Pressed:
						color = Color::red;
						break;
					}
				}

				FVec2 position = transformComponent.position + boundingBoxComponent.offset;
				const FVec2 size = boundingBoxComponent.halfSize * 2.f;

				position -= size * transformComponent.pivot;

				m_renderer.drawOutlineRect(position.x, position.y, size.x, size.y, color);
			});*/
	}
}