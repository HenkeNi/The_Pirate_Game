#pragma once
#include "System.h"
#include "engine/resources/engine_resources.h"

namespace cursed_engine
{
	class AssetManager;
	class Renderer;

	class RenderSystem : public System
	{
	public:
		RenderSystem(Renderer& renderer, EngineResources& engineResources, AssetManager& assetManager);

		void update(ECSRegistry& registry, float deltaTime) override;
	
	private:
		void renderText(ECSRegistry& registry);
		void renderDebug(ECSRegistry& registry);

		Renderer& m_renderer;
		EngineResources& m_engineResources;
		AssetManager& m_assetManager;
	};
}