#include "engine/ecs/system/screen_space_render_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"

namespace cursed_engine
{
	ScreenSpaceRenderSystem::ScreenSpaceRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer)
		: m_textureManager{ std::move(textureManager) }, m_assetManager{ assetManager }, m_renderer{ std::move(renderer) }
	{
	}

	void ScreenSpaceRenderSystem::update(SystemContext& context)
	{
		m_renderer.setRenderState(cursed_engine::RenderState{ cursed_engine::View{ { 0.f, 0.f } , 0.0, 1.f}, cursed_engine::Projection{ { 1280.f, 720.f }, { 0.f, 0.f } } }); // view, projection				

		renderSprites(context.registry);
		renderDebug(context.registry);
		renderText(context.registry);
	}

	void ScreenSpaceRenderSystem::renderSprites(ECSRegistry& registry)
	{
		const auto componentView = registry.view<SpriteComponent, TransformComponent, UIComponent>();
		componentView.forEach([&](const SpriteComponent& spriteComponent, const TransformComponent& transformComponent, const UIComponent&) // TODO; handle filtering... (UIComponent not used)
			{
				const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

				const auto& textureHandle = m_textureManager->getHandleById(textureAtlas.textureId);

				if (auto* texture = m_textureManager->get(textureHandle))
				{
					auto position = transformComponent.position;
					const auto& scale = transformComponent.scale;

					position -= scale * transformComponent.pivot;

					// TODO; use correct region!

					m_renderer.drawTexture(*texture, position, scale, spriteComponent.color);
				}
			});
	}

	void ScreenSpaceRenderSystem::renderDebug(ECSRegistry& registry)
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

	void ScreenSpaceRenderSystem::renderText(ECSRegistry& registry)
	{
		auto view = registry.view<TransformComponent, TextComponent, UIComponent>();

		//// TODO; check if possible to have one argument const ref and one argument just ref...
		view.forEach([&](const TransformComponent& transformComponent, TextComponent& textComponent, const UIComponent&)
			{
				FVec2 position = transformComponent.position;
				const FVec2 size = (FVec2)textComponent.textObj.getSize();
				
				position -= size * transformComponent.pivot;

				m_renderer.drawText(textComponent.textObj, position.x, position.y);
			});
	}
}