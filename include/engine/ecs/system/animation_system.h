#pragma once
#include "engine/ecs/system/system.h"

namespace cursed_engine
{
	class AssetManager;

	class AnimationSystem : public UpdateSystem
	{
	public:
		AnimationSystem(AssetManager& assetManager);

		void update(SystemUpdateContext& context) override;
	
	private:
		AssetManager& m_assetManager;
	};
}