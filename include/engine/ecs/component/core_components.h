#pragma region
#include "engine/math/vec2.h"
#include "engine/assets/asset_types.h"
#include "engine/assets/asset_manager.h" // TODO; put in asset handle file instead?
#include "engine/rendering/animation_types.h"
#include <unordered_map>

namespace cursed_engine
{
	struct TransformComponent
	{
		// TODO; add 2 constructors? one for each individual argument?

		FVec2 position{ 0.f, 0.f };
		FVec2 scale{ 1.f, 1.f };
		FVec2 pivot{ 0.f, 0.f };
		float rotation = 0.f;
	};

	struct VelocityComponent
	{
		FVec2 velocity = { 0.f, 0.f };
		float baseSpeed = 1.f;
		float currentSpeed = 0.f;
		float speedMultiplier = 1.f;
		float speedReductionRate = 20.f;  // rename...
		bool isVelocityConstant = false; // Dont? use physics instead?
	};

	struct CameraComponent
	{
		FVec2 position = { 0.f, 0.f };
		float aspectRatio = 0.f;
		float rotation = 0.f;
		float zoom = 1.f;

		bool isActive = true;
	};

	struct SpriteComponent
	{
		AssetHandle atlasHandle; // Handle to texture atlas
		AtlasRegion region;
		float colors[4];
		float zOrder;
	};

	struct AnimationComponent
	{
		AssetHandle spriteSheetHandle; // const ref?
		
		// TODO; decide what to store in component vs animation (if should have it at all)
		//std::unordered_map<std::string, Animation> animations;
		
		Animation animation;  // poionter to an animation? store animations in asset manager?
		int currentFrameIndex = 0;

		float elapsedFrameTime = 0.f;
		bool isDone = false;
		bool isPlaying = false;
		bool isLooping = false;
	};

	struct InputComponent
	{
	};
}