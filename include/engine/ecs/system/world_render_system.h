#pragma once
#include "engine/ecs/system/system.h"
#include "engine/rendering/render_api.h"
#include "engine/resources/resources.h"
#include "engine/math/aabb.hpp"


#include "engine/ecs/entity/entity.h"

namespace cursed_engine
{
	class AssetManager;
	class ECSRegistry;
	class PhysicsDebugDraw;

	class WorldRenderSystem : public RenderSystem
	{
	public:
		WorldRenderSystem(Resources resources, AssetManager* assetManager, RenderAPI renderer, PhysicsDebugDraw* physicsDebugDraw = nullptr);

		void render(SystemRenderContext& context) override;
	
	private:
		void renderSprites(ECSRegistry& registry, const FAABB& viewBounds);
		void renderDebug(ECSRegistry& registry, const FAABB& viewBounds);

		AssetManager* m_assetManager;
		PhysicsDebugDraw* m_physicsDebugDraw;
		RenderAPI m_renderer;
		Resources m_resources;
	
		std::vector<Entity> m_entityBuffer;
	};
}