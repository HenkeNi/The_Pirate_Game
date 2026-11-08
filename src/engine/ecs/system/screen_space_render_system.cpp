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

	ScreenSpaceRenderSystem::ScreenSpaceRenderSystem(Resources resources, AssetManager* assetManager, RenderAPI renderer)
		: m_resources{ resources }, m_assetManager{ assetManager }, m_renderer{ std::move(renderer) }
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
		const auto componentView = registry.view<SpriteComponent, UITransformComponent>();
		componentView.forEach([&](const SpriteComponent& spriteComponent, const UITransformComponent& uiTransformComponent)
			{
				const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

				const auto& textureHandle = m_resources.getHandleById<Texture>(textureAtlas.textureId);

				if (auto* texture = m_resources.tryGet<Texture>(textureHandle))
				{
					// so if user specifies ancohr points, they are used, if not - the region size?

					FVec2 position = uiTransformComponent.position;
					FVec2 size = uiTransformComponent.size;
					//FVec2 scale = uiTransformComponent.scale;

					const AtlasRegion& region = spriteComponent.atlasRegion;
					//const FVec2 scaledSize = region.getSize() * scale;

					position -= region.pivot * size;
					//position -= region.pivot * scaledSize;

					FRect src = (FRect)region.rect;
					//FRect dst{ position.x, position.y, scaledSize.x, scaledSize.y };
					FRect dst{ position.x, position.y, size.x, size.y };

					m_renderer.drawTexture(*texture, std::move(src), std::move(dst), spriteComponent.color);
					//m_renderer.drawTexture(*texture, std::move(src), screenRectComponent.rect, spriteComponent.color);
				}
			});
	}

	//void ScreenSpaceRenderSystem::renderSprites(ECSRegistry& registry)
	//{
	//	const auto componentView = registry.view<SpriteComponent, WorldTransformComponent, UIComponent>();
	//	componentView.forEach([&](const SpriteComponent& spriteComponent, const WorldTransformComponent& transformComponent, const UIComponent&) // TODO; handle filtering... (UIComponent not used)
	//		{
	//			const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

	//			const auto& textureHandle = m_textureManager->getHandleById(textureAtlas.textureId);

	//			if (auto* texture = m_textureManager->tryGet(textureHandle))
	//			{
	//				FVec2 position = transformComponent.position;

	//				const AtlasRegion& region = spriteComponent.atlasRegion;
	//				const FVec2 scaledSize = region.getSize() * transformComponent.scale;

	//				position -= region.pivot * scaledSize;

	//				FRect src = (FRect)region.rect;
	//				FRect dst{ position.x, position.y, scaledSize.x, scaledSize.y };

	//				m_renderer.drawTexture(*texture, std::move(src), std::move(dst), spriteComponent.color);
	//			}
	//		});
	//}

	void ScreenSpaceRenderSystem::renderDebug(ECSRegistry& registry)
	{
		auto view = registry.view<WorldTransformComponent, BoundingBoxComponent, UIComponent>();
		view.forEach([&](Entity entity, WorldTransformComponent& transformComponent, BoundingBoxComponent& boundingBoxComponent, const UIComponent&)
			{
				Color color = Color::green;

				if (auto* uiInteractableComponent = registry.tryGetComponent<UIInteractableComponent>(entity))
				{
					switch (uiInteractableComponent->currentState)
					{
					case UIInteractableComponent::State::Normal:
						color = Color::green;
						break;

					case UIInteractableComponent::State::Hovered:
						color = Color::yellow;
						break;

					case UIInteractableComponent::State::Pressed:
						color = Color::red;
						break;
					}
				}

				const FVec2 center = transformComponent.position + boundingBoxComponent.offset;
				const FVec2 halfSize = boundingBoxComponent.size * 0.5f;

				FRect rect
				{
					center.x - halfSize.x,
					center.y - halfSize.y,
					boundingBoxComponent.size.x,
					boundingBoxComponent.size.y
				};

				m_renderer.drawOutlineRect(rect, color);
				//m_renderer.drawOutlineRect(
				//	topLeft.x,
				//	topLeft.y,
				//	halfSize.x * 2.0f,
				//	halfSize.y * 2.0f,
				//	color
				//);
			});
	}

	void ScreenSpaceRenderSystem::renderText(ECSRegistry& registry)
	{
		auto view = registry.view<WorldTransformComponent, TextComponent, UIComponent>();

		//// TODO; check if possible to have one argument const ref and one argument just ref...
		view.forEach([&](const WorldTransformComponent& transformComponent, TextComponent& textComponent, const UIComponent&)
			{
				if (!textComponent.text)
				{
					// Log error!?					
					return;
				}

				Text& text = *textComponent.text;

				FVec2 position = transformComponent.position;
				const FVec2 scaledSize = (FVec2)text.getSize() * transformComponent.scale;

				position -= textComponent.pivot * scaledSize;

				m_renderer.drawText(text, position.x, position.y);
			});
	}
}