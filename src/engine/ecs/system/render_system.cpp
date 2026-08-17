#include "engine/ecs/system/render_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"

#include "engine/resources/resource_manager.hpp"
#include "engine/resources/resource_types.h"
#include "engine/resources/texture/texture.h"

#include "engine/rendering/render_api.h"
#include "engine/rendering/render_types.h" // or put in render api?

namespace cursed_engine
{
	RenderSystem::RenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer)
		: m_textureManager{ textureManager }, m_assetManager{ assetManager }, m_renderer{ renderer }
	{
	}

	void RenderSystem::update(SystemContext& context)
	{
		auto& registry = context.registry;
		
		auto view = registry.view<CameraComponent>();
		auto activeCamera = view.findFirst([](const CameraComponent& cameraComponent)
			{
				return cameraComponent.isActive;
			});

		FVec2 worldPosition{}; // create FVec2::zero();??

		if (activeCamera.has_value())
		{
			const auto& cameraTransformComponent = registry.getComponent<TransformComponent>(activeCamera.value());
			worldPosition = cameraTransformComponent.position;

			m_renderer.setRenderState(cursed_engine::RenderState{ cursed_engine::View{worldPosition, 0.0, 0.f}, cursed_engine::Projection{ { 1280, 720 } } }); // view, projection				
		}

		// Pass camera pos to a function? if no camera, pass 0, 0?

		// TODO; sort by handles? -> maybe sort using texture id (perhaps integer would be better than string?)

		//auto& textureManager = m_engineResources.textureManager;

		renderTextures(registry, worldPosition);
	

		// ################### TEST #########################

#ifndef NDEBUG

		renderDebug(registry);

#endif

		//m_renderer.drawLine(0, 0, 100, 100, Color{ 123, 21, 32, 255 });

		renderText(registry);
		//m_renderer.present(); // Here or in main loop?

		//renderText(registry); -> maybe not needed? text uses textuers...
	}

	void RenderSystem::renderTextures(ECSRegistry& registry, const FVec2& cameraPos)
	{
		const auto componentView = registry.view<SpriteComponent, TransformComponent>();
		componentView.forEach([&](const SpriteComponent& spriteComponent, const TransformComponent& transformComponent)
			{
				const auto& textureAtlas = m_assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle); // no asset stored AND invalid index!

				const auto& textureHandle = m_textureManager->getHandleById(textureAtlas.textureId);

				if (auto* texture = m_textureManager->get(textureHandle))
				{
					// worldPos = transform.position 
					// -(pivot * sprite.size * transform.scale)

					// TODO; handle invalid/not loaded texture!

					//const auto& pos = transformComponent.position;
					//const auto& scale = transformComponent.scale;
					auto position = transformComponent.position;
					position.x -= cameraPos.x;
					position.y -= cameraPos.y;

					const auto& scale = transformComponent.scale;

					position -= scale * transformComponent.pivot;

					m_renderer.drawTexture(*texture, position, scale, spriteComponent.color);
				}
			});
	}

	void RenderSystem::renderText(ECSRegistry& registry)
	{
		auto view = registry.view<TransformComponent, TextComponent>();

		// TODO; check if possible to have one argument const ref and one argument just ref...
		view.forEach([&](const TransformComponent& transformComponent, TextComponent& textComponent)
			{	
				//FVec2 position = transformComponent.position + boundingBoxComponent.offset;
				//FVec2 size = boundingBoxComponent.halfSize * 2.f;
				FVec2 position = transformComponent.position;
				FVec2 size = (FVec2)textComponent.textObj.getSize();
				position -= size * transformComponent.pivot;


				m_renderer.drawText(textComponent.textObj, position.x, position.y);

				return;

				auto textureHandle = textComponent.textureHandle;

				//if (!textureHandle.isValid())
				//{
				//	Logger::logError("[RenderSystem::renderText] - Invalid texture handle");
				//	textComponent.isDirty = true;
				//	//(false && "Invalid (text) texture handle"); // allow since text system generates new handle...

				//	return;
				//}

				////if (auto* texture = m_engineResources.textureManager.get(textureHandle))
				//if (auto* texture = m_engineResources.textManager.get(textureHandle))
				//{
				//	const auto& position = transformComponent.position;
				//	const auto& size = transformComponent.scale; // this?
				//	m_renderer.renderTexture(position, size, *texture, textComponent.color);
				//}
				//else
				//{
				//	textComponent.isDirty = true;
				//}

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

		// TODO; render system shouldnt know about buttons!!!!!!!!!!!!!!!
		auto view = registry.view<TransformComponent, BoundingBoxComponent>();
		view.forEach([&](Entity entity, TransformComponent& transformComponent, BoundingBoxComponent& boundingBoxComponent)
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
				FVec2 size = boundingBoxComponent.halfSize * 2.f;
				position -= size * transformComponent.pivot;

				m_renderer.drawOutlineRect(position.x, position.y, size.x, size.y, color);
			});
	}
}