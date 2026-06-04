#pragma once
#include "System.h"
#include "engine/resources/resource_types.h"

namespace cursed_engine
{
	// TODO; fix render depth...

	class AssetManager;
	class RenderAPI;
	//class TextureManager;
	//class TextRenderer;

	class RenderSystem : public System
	{
	public:
		RenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI* renderer);

		void update(SystemContext& context) override;
	
	private:
		void renderText(ECSRegistry& registry);
		void renderDebug(ECSRegistry& registry);

		TextureManager* m_textureManager;
		AssetManager* m_assetManager;
		RenderAPI* m_renderer;
	};
}