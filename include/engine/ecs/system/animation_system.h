#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class AssetManager;

	class AnimationSystem : public System
	{
	public:
		AnimationSystem(AssetManager& assetManager);

		void update(ECSRegistry& registry, float deltaTime) override;
	
	private:
		AssetManager& m_assetManager;
	};
}