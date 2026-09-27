#include "engine/ecs/system/screen_space_render_system.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/ecs_registry.h"

namespace cursed_engine
{
#ifdef _DEBUG

	static void renderTest(RenderAPI& renderer, ECSRegistry& registry)
	{
		//renderer.drawOutlineCircle(200, 200, 100);
		//renderer.drawFillCircle(400, 400, 50);
	}

#endif

	ScreenSpaceRenderSystem::ScreenSpaceRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer)
		: m_textureManager{ std::move(textureManager) }, m_assetManager{ assetManager }, m_renderer{ std::move(renderer) }
	{
	}

	void ScreenSpaceRenderSystem::render(SystemRenderContext& context)
	{
		m_renderer.setRenderState(cursed_engine::RenderState{ cursed_engine::View{ { 0.f, 0.f } , 0.0, 1.f}, cursed_engine::Projection{ { 1280.f, 720.f }, { 0.f, 0.f } } }); // view, projection				

		renderSprites(context.registry);

#ifdef _DEBUG

		renderDebug(context.registry);
		renderTest(m_renderer, context.registry);

#endif // _DEBUG

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
					/*FVec2 position = transformComponent.position;

					const FVec2 scaledSize = spriteComponent.atlasRegion.getSize() * transformComponent.scale;
					position -= spriteComponent.atlasRegion.pivot * scaledSize;*/

					FVec2 position = transformComponent.position;

					const AtlasRegion& region = spriteComponent.atlasRegion;
					const FVec2 scaledSize = region.getSize() * transformComponent.scale;

					position -= region.pivot * scaledSize;

					//position -= scale * region.pivot;
					//position = computeDrawPosition(position, , region.pivot); // HOTPATH? Dont call compute? dont construct FVec for size?

					FRect src = (FRect)region.rect;
					//FRect dst{ position.x, position.y, region.getSize().x, region.getSize().y};
					FRect dst{ position.x, position.y, scaledSize.x, scaledSize.y };

					m_renderer.drawTexture(*texture, std::move(src), std::move(dst), spriteComponent.color);

					/*auto position = transformComponent.position;
					const auto& scale = transformComponent.scale;

					position -= scale * transformComponent.pivot;*/

					// TODO; use correct region!

					//m_renderer.drawTexture(*texture, position, scaledSize, spriteComponent.color);
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

				const FVec2 halfSize =
					boundingBoxComponent.halfSize * transformComponent.scale; // should scale affect size?

				const FVec2 topLeft =
					transformComponent.position + boundingBoxComponent.offset - halfSize;

				m_renderer.drawOutlineRect(
					topLeft.x,
					topLeft.y,
					halfSize.x * 2.0f,
					halfSize.y * 2.0f,
					color
				);
			/*	FVec2 position = transformComponent.position + boundingBoxComponent.offset;
				const FVec2 size = boundingBoxComponent.halfSize * transformComponent.scale;

				position -= size;

				m_renderer.drawOutlineRect(position.x, position.y, size.x * 2.0f, size.y * 2.0f, color);*/
			});
	}

	void ScreenSpaceRenderSystem::renderText(ECSRegistry& registry)
	{
		auto view = registry.view<TransformComponent, TextComponent, UIComponent>();

		//// TODO; check if possible to have one argument const ref and one argument just ref...
		view.forEach([&](const TransformComponent& transformComponent, TextComponent& textComponent, const UIComponent&)
			{
				FVec2 position = transformComponent.position;
				const FVec2 scaledSize = (FVec2)textComponent.textObj.getSize() * transformComponent.scale;

				position -= textComponent.pivot * scaledSize;

				m_renderer.drawText(textComponent.textObj, position.x, position.y);
			});
	}
}