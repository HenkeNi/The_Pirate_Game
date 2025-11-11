#pragma region
#include "engine/math/vec2.h"

namespace cursed_engine
{
	struct TransformComponent
	{
		FVec2 position{ 0.f, 0.f };
		FVec2 scale{ 1.f, 1.f };
		FVec2 pivot{ 0.f, 0.f };
		float rotation = 0.f;
	};

	struct VelocityComponent
	{
		FVec2 velocity;
		float baseSpeed = 1.f;
		float currentSpeed = 1.f;
		float speedMultiplier = 1.f;
		float speedReductionRate = 20.f;  // rename...
		bool isVelocityConstant = false; // Dont? use physics instead?
	};

	struct CameraComponent
	{
		FVec2 position;
		float aspectRatio;
		float rotation;
		float zoom;

		bool isActive = true;
	};

	struct SpriteComponent
	{
		std::string textureID; 
		IVec2 coordinates;
	};

	struct InputComponent
	{

	};
}