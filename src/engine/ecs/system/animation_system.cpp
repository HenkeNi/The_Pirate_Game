#include "engine/ecs/system/animation_system.h"
#include "engine/ecs/ecs_registry.h"
#include "engine/ecs/component/core_components.h"
#include "engine/assets/asset_manager.h"
#include "engine/assets/asset_types.h"

namespace cursed_engine
{
	AnimationSystem::AnimationSystem(AssetManager& assetManager)
		: m_assetManager{ assetManager }
	{
	}

	void AnimationSystem::update(SystemContext& context)
	{
		auto view = context.registry.view<AnimationComponent, SpriteComponent>();

		view.forEach([&](AnimationComponent& animationComponent, SpriteComponent& spriteComponent)
			{
				if (animationComponent.isFinished)
					return;

				animationComponent.elapsedTime += context.deltaTime;

				const AnimationSet& animationSet = m_assetManager.getAsset<AnimationSet>(animationComponent.animationSetHandle);

				const std::unordered_map<std::string, Animation>& animations = animationSet.animations;

				if (auto it = animations.find(animationComponent.currentAnimationId); it != animations.end())
				{
					const Animation& animation = it->second;
					std::size_t& frameIndex = animationComponent.currentFrameIndex;

					const Animation::Frame& frame = animation.frames.at(frameIndex);

					if (animationComponent.elapsedTime >= frame.duration)
					{
						animationComponent.elapsedTime = 0.f; // or last?
						++frameIndex;

						if (frameIndex >= animation.frames.size())
						{
							if (!animation.looping)
							{
								animationComponent.isFinished = true;
								return;
							}

							frameIndex = 0;
						}


						const Animation::Frame& nextFrame = animation.frames.at(frameIndex);

						const AssetHandle textureAtlasHandle = m_assetManager.getAssetHandle<TextureAtlas>(animationSet.textureId);
						const TextureAtlas& textureAtlas = m_assetManager.getAsset<TextureAtlas>(textureAtlasHandle);

						// store texture atlas handle in frame?
						if (auto it = textureAtlas.idToRegionIndex.find(nextFrame.regionId); it != textureAtlas.idToRegionIndex.end())
						{
							spriteComponent.atlasRegion = textureAtlas.regions[it->second];
						}
						else
						{
							Logger::logError("[AnimationSystem::update] - Failed to locate atlas region!");
							return;
						}
					}
				}
				else
				{
					Logger::logError("[AnimationSystem::update] - Failed to locate animation!");
					return;
				}
			});
	}
}