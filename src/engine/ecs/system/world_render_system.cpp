#include "engine/ecs/system/world_render_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/rendering/render_types.h"

namespace cursed_engine
{
	WorldRenderSystem::WorldRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer)
		: m_textureManager{ std::move(textureManager) }, m_assetManager{ assetManager }, m_renderer{ std::move(renderer) }
	{ 
	}

	void WorldRenderSystem::update(SystemContext& context)
	{
		auto view = context.registry.view<CameraComponent>();

		auto activeCamera = view.findFirst([](const CameraComponent& cameraComponent)
			{
				return cameraComponent.isActive;
			});

		FVec2 worldPosition{}; // create FVec2::zero();??

		if (activeCamera.has_value())
		{
			const auto& cameraTransformComponent = context.registry.getComponent<cursed_engine::TransformComponent>(activeCamera.value());
			worldPosition = cameraTransformComponent.position;

			//Logger::logInfo(std::format("{} {}", worldPosition.x, worldPosition.y));
		}

		m_renderer.setRenderState(RenderState{ { worldPosition , 0.0, 1.f}, Projection{ FVec2{ 1280.f, 720.f }, FVec2{ 0.f, 0.f } } }); // view, projection				

		renderSprites(context.registry);
		renderDebug(context.registry);
	}

	void WorldRenderSystem::renderSprites(ECSRegistry& registry)
	{
		// Filter out ui component entities

		auto view = registry.view<SpriteComponent, TransformComponent>();
		view.exclude<UIComponent>();

		view.forEach([&](const SpriteComponent& spriteComponent, const TransformComponent& transformComponent) // TODO; handle filtering... (UIComponent not used)
			{
				const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

				const auto& textureHandle = m_textureManager->getHandleById(textureAtlas.textureId);

				if (auto* texture = m_textureManager->get(textureHandle))
				{
					FVec2 position = transformComponent.position;
					const FVec2& scale = transformComponent.scale;

					position -= scale * transformComponent.pivot;

					FRect src = (FRect)spriteComponent.atlasRegion.rect;
					FRect dst{ position.x, position.y, scale.x, scale.y };

					m_renderer.drawTexture(*texture, std::move(src), std::move(dst), spriteComponent.color);
				}
			});
	}

	void WorldRenderSystem::renderDebug(ECSRegistry& registry)
	{
		auto view = registry.view<TransformComponent, BoundingBoxComponent, UIComponent>();
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
			});
	}
}