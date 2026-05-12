#pragma once
#include "System.h"
#include "engine/resources/engine_resources.h"

namespace cursed_engine
{
	// TODO; fix render depth...

	class AssetManager;
	class Renderer;
	class TextRenderer;

	class RenderSystem : public System
	{
	public:
		RenderSystem(EngineResources& engineResources, AssetManager& assetManager, Renderer& renderer, TextRenderer& textRenderer);

		void update(SystemContext& context) override;
	
	private:
		void renderText(ECSRegistry& registry);
		void renderDebug(ECSRegistry& registry);

		EngineResources& m_engineResources;
		AssetManager& m_assetManager;
		TextRenderer& m_textRenderer;
		Renderer& m_renderer;
	};
}