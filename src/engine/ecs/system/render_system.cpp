#include "engine/ecs/system/render_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/rendering/renderer.h"
#include "engine/resources/resource_manager.hpp"
#include "engine/rendering/texture.h"

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

		auto componentView = registry.view<SpriteComponent, TransformComponent>();
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

		auto textureHandleTest = m_engineResources.getHandle<Texture>("test3"); // pass in "coordinates"? or do that in texture?
		auto& testTexture = m_engineResources.get<Texture>(textureHandleTest);

		m_renderer.renderTexture(10, 10, 10, 10, testTexture);
		m_renderer.renderLine(0, 0, 100, 100);

		m_renderer.present(); // Here or in main loop?
	}
}