#pragma region
#include "engine/assets/asset_types.h"
#include "engine/assets/asset_manager.h" // AssetHandle
#include "engine/core/action/action_registry.h" // or put type alisas in action.h?
#include "engine/ecs/entity/entity_handle.h"
#include "engine/math/vec2.hpp"
#include "engine/math/aabb.hpp"
#include "engine/physics/physics.h"
#include "engine/physics/physics_types.h"
#include "engine/rendering/animation/animation_types.h"
#include "engine/rendering/render_types.h" // remove?
#include "engine/rendering/text/text.h"

#include "engine/resources/resource_types.h"

#include <array>
#include <optional>
#include <unordered_map>

namespace cursed_engine
{
	// Consider TransformComponent; have localPosition, localScale, localRotation instead?
	// UITransformComponent as well?

	// Core
	struct TransformComponent
	{
		FVec2 position{ 0.f, 0.f };
		FVec2 scale{ 1.f, 1.f }; // use this as scale not size!!!
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

	// here or game?
	struct CameraComponent
	{
		CameraComponent() = default;

		CameraComponent(IVec2 viewport)
			: viewportSize{ viewport }
		{
		}

		FVec2 position = { 0.f, 0.f }; // offset instead? or use heirarchy component..
		float aspectRatio = 0.f;
		float rotation = 0.f;
		float zoom = 1.f;

		// FRect rect; // or i Rect? or just size and width?
		IVec2 viewportSize; // update when screen changes?! camera system?

		// DONT USE FRECT?? bit confusing that width and height are not actual width and heights but positions
		//FRect viewport; // TEST! -> if using, maybe could use when rendering as well... USE BOUNDS? or calculate each frame?
		Bounds bounds;

		bool isActive = true;
	};

	struct SpriteComponent
	{
		AssetHandle atlasHandle; // id to atlas instead?
		AtlasRegion atlasRegion; // source rect? // use region ID instead? - find better way to handle single textures than 

		Color color = Color::white;
		// float zIndex = 0.f;
	};

	struct AnimationComponent
	{
		AnimationComponent() = default;
		AnimationComponent(AssetHandle handle, std::string currentAnimationId)
			: animationSetHandle{ std::move(handle) }, currentAnimationId{ std::move(currentAnimationId) }
		{
		}

		AssetHandle animationSetHandle;
		std::string currentAnimationId{};

		float elapsedTime = 0.f;
		std::size_t currentFrameIndex = 0; // unsigned?

		bool isFinished = false;
	};

	struct UIComponent
	{
	};

	class Audio;
	struct AudioComponent
	{
		ResourceHandle<Audio> audioHandle;
		bool isLooping;
	};

	// Not needed? maybe dont make any sense? (in menu, where to attach to?)
	//struct InputComponent
	//{
	//	//std::unordered_map<Hi_Engine::eKey, bool> InputStates; // replace with state instead of bool?? rename KeyStates?
	//	//FVector2 MousePosition;
	//	//FVector2 MouseWorldPosition;
	//	//float MouseScroll;
	//	FVec2 mousePosition;
	//};

	// or BoundsComponent?
	struct BoundingBoxComponent
	{
		FVec2 offset;
		FVec2 halfSize;
	};

	struct HierarchyComponent // or name ParentComponent
	{
		HierarchyComponent() = default;
		
		HierarchyComponent(FVec2 offset)
			: offsetToParent{ offset }
		{
		}

		FVec2 offsetToParent;
		// handles? optionals?
		EntityHandle parent = EntityHandle::invalid();
		EntityHandle nextSibling = EntityHandle::invalid();
		EntityHandle prevSibling = EntityHandle::invalid();
		EntityHandle firstChild = EntityHandle::invalid();
		//	// also children?
	};

	// interactable?
	struct ButtonComponent
	{
		//using ActionValue = std::variant<std::string, int, double, bool>;

		ButtonComponent() = default;
		ButtonComponent(std::string action, ActionArgs args, Color defaultColor, std::optional<Color> hoverColor = std::nullopt, std::optional<Color> pressedColor = std::nullopt)
			: action{ action }, actionArgs{ std::move(args) }, defaultColor{ defaultColor }, hoverColor{ hoverColor }, pressedColor{ pressedColor }
		{
		}

		enum class State { Normal, Hovered, Pressed }; // replace with input state instead?
		State currentState = State::Normal;
		State previousState = State::Normal;

		// Make into an Action (struct)?
		std::string action; // dont use string?
		ActionArgs actionArgs;
		//ActionValue actionValue; // use vector of ActionParams?

		Color defaultColor;
		std::optional<Color> hoverColor;
		std::optional<Color> pressedColor;

		// on click...? function pointer? or send event?
	};

	class Texture;
	class Font;

	struct TextComponent
	{
		TextComponent() = default;

		TextComponent(TextPtr&& text, FVec2 pivot)
			: text{ std::move(text) }, pivot{ pivot }
		{
		}

		TextPtr text; 
		FVec2 pivot{}; // or store in text class?
	};

	// StackPanelComponent?
	struct LayoutComponent
	{
		// UI types? or where?
		enum class Direction
		{
			Horizontal, Vertical
		};

		Direction direction;
		// vector of entities? or children have a parent? or have a HeartContainerComponnet?
	};

	//struct RepeaterComponent

	struct CheckboxComponent
	{
		//ResourceHandle<Texture> uncheckedTexture;
		//ResourceHandle<Texture> checkedTexture;

		bool isChecked;
	};

	struct SliderComponent
	{
		/*SliderComponent(Orientation orientation, float minValue, float maxValue, float currentValue = 0.f)
			: orientation{ orientation }, minValue{ minValue }, maxValue{ maxValue }, currentValue{ currentValue }
		{
		}*/

		// cant make const? needs to be able to be initialized form other 
		Orientation orientation;
		float minValue; // or line?
		float maxValue;

		float currentValue;
	};

	struct SliderHandleComponent
	{
		// 
	};

	// or TargetComponent??
	struct FollowComponent
	{
		EntityHandle target = EntityHandle::invalid();
	};

	struct InputFieldComponent // or TextField
	{
	};

	struct RadioButton
	{
	};

	struct Dropdown
	{
	};

	struct Switch // Or Toggle
	{
	};

	struct PhysicsComponent
	{
		PhysicsComponent() = default;

		PhysicsComponent(BodyDefinition bodyDefinition)
			: bodyDefinition{ std::move(bodyDefinition) }
		{
		}

		BodyDefinition bodyDefinition; // Good to store, or only for creation?
		PhysicsBody physicsBody = PhysicsBody::invalid();
	};


	struct BehaviorTreeComponent
	{
	};

	//struct TransformComponent
	//{
	//	FVec2 localPosition{ 0.f, 0.f };
	//	FVec2 localScale{ 1.f, 1.f };
	//	float localRotation = 0.f;
	//	
	//	FVec2 pivot{ 0.f, 0.f }; // helper functions or add funct in struct?
	//
	//	mat4 worldTransform; // dont store? compute?
	//};

	//struct RenderLayer
	//{
	//	Texture* texture;
	//	Vector2 offset;
	//	float rotation = 0.0f;
	//	Vector2 scale = { 1,1 };
	//	int order = 0;
	//};

	//struct RenderComponent
	//{
	//	std::vector<RenderLayer> layers;
	//};

	// Tab? View? Tooltip? ProgressBar?
}