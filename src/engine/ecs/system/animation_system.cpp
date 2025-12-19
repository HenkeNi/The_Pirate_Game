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

	void AnimationSystem::update(ECSRegistry& registry, float deltaTime)
	{
		 auto view = registry.view<AnimationComponent, SpriteComponent>();
		 //auto view = registry.view<TransformComponent, SpriteComponent>();
		


		//auto view = registry.view<SpriteComponent, AnimationComponent>();

		/*view.forEach([=](SpriteComponent& spriteComponent, AnimationComponent& animationComponent)
			{
				if (animationComponent.isDone && !animationComponent.isLooping)
					return;

				float& elapsed = animationComponent.elapsedFrameTime;
				elapsed += deltaTime;

				int& frameIndex = animationComponent.currentFrameIndex;


				// Does animation componnet need to store any spritesheet at all? or just use texture handle in... => spritesheet and texture atlass not the same! 


				//const Animation& animation = animationComponent.animation;
				//auto currentFrame = animation.frames.at(frameIndex);

				if (elapsed >= currentFrame.duration)
				{
					++frameIndex;	// TODO; wrap around (if reached end AND should loop, else mark is done) fire event?
					
					if (frameIndex >= animation.frames.size() && animationComponent.isLooping)
					{
						frameIndex = 0;
					}

					// TODO; fetch spritesheet...
					//const SpriteSheet& spriteSheet = m_assetManager.getAsset<SpriteSheet>(animationComponent.spritesheetHandle);
					//spriteSheet.textureAtlas


					// TODO; Update sprite...
					//const auto& handle = animationComponent.spritesheetHandle;

					elapsed = 0.f; // update elapsed time with % remaining?
				}
			});*/
	}
}