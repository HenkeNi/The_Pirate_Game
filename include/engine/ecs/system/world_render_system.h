#pragma once
#include "engine/ecs/system/system.h"
#include "engine/resources/resource_types.h"
#include "engine/rendering/render_api.h"
#include "engine/math/aabb.hpp"


#include "engine/ecs/entity/entity.h"

namespace cursed_engine
{
	class AssetManager;
	class ECSRegistry;
	class PhysicsDebugDraw;

	class WorldRenderSystem : public System
	{
	public:
		WorldRenderSystem(TextureManager* textureManager, AssetManager* assetManager, RenderAPI renderer, PhysicsDebugDraw* physicsDebugDraw = nullptr);

		void update(SystemContext& context) override;
	
	private:
		void renderSprites(ECSRegistry& registry, const FAABB& viewBounds);
		void renderDebug(ECSRegistry& registry, const FAABB& viewBounds);

		AssetManager* m_assetManager;
		TextureManager* m_textureManager;
		PhysicsDebugDraw* m_physicsDebugDraw;
		RenderAPI m_renderer;
	
		std::vector<Entity> m_entityBuffer;
	};
}