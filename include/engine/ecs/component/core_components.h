#pragma region
#include "engine/math/vec2.h"
#include "engine/assets/asset_types.h"
#include "engine/assets/asset_manager.h" // remove include?
#include "engine/rendering/animation_types.h"
#include "engine/rendering/render_types.h"
#include <optional>
#include <array>
#include <unordered_map>

namespace cursed_engine
{
	class ComponentRegistry; 
	class EngineResources;
	struct ResourceConfig;

	namespace core_components
	{
		void registerAll(ComponentRegistry& registry, AssetManager& assetManager, EngineResources& engineResources, const ResourceConfig& resourceConfig);
	}

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
		//SpriteComponent(AtlasH handle, AtlasRegion region, float color[4], float z)
		//	: atlasH

		AssetHandle atlasHandle; // Handle to texture                   --- better than storing id to atlas handle? (maybe to much indirection)
		AtlasRegion atlasRegion;
		std::array<float, 4> color; // use COlor instead?
		//float colors[4]; // create type or type alias for color!

		// store texture handle? if invalid, overwrite with new one from getHandle...

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

	// Not needed? maybe dont make any sense? (in menu, where to attach to?)
	//struct InputComponent
	//{
	//	//std::unordered_map<Hi_Engine::eKey, bool> InputStates; // replace with state instead of bool?? rename KeyStates?
	//	//FVector2 MousePosition;
	//	//FVector2 MouseWorldPosition;
	//	//float MouseScroll;
	//	FVec2 mousePosition;
	//};

	struct BoundingBox
	{
		FVec2 offset;
		FVec2 halfSize;

		//Vec2 center;
		//Vec2 halfSize;

		//Vec2 min() const { return center - halfSize; }
		//Vec2 max() const { return center + halfSize; }
		//Vec2 size() const { return halfSize * 2.0f; }

		//FVec2 position;
		//int xOffset, yOffset;
		//float x, y; // or offset instead?
		//int width, height;

		// rotation?
	};


	struct ButtonAction
	{
		// onHover
		// onPress or onClick
	};


	// interactable?
	struct ButtonComponent
	{
		using ActionValue = std::variant<std::string, int, double, bool>;

		ButtonComponent() = default;
		ButtonComponent(std::string action, ActionValue value, Color defaultColor, std::optional<Color> hoverColor = std::nullopt, std::optional<Color> pressedColor = std::nullopt)
			: action{ action }, actionValue{ value }, defaultColor{ defaultColor }, hoverColor{ hoverColor }, pressedColor{ pressedColor }
		{
		}

		enum class State{ Normal, Hovered, Pressed };
		State currentState = State::Normal;
		State previousState = State::Normal;

		std::string action; // dont use string?
		ActionValue actionValue;

		Color defaultColor;
		std::optional<Color> hoverColor;
		std::optional<Color> pressedColor;

		// on click...? function pointer? or send event?
	};
	
	class Texture;
	class Font;

	struct TextComponent 
	{
		TextComponent(std::string id, ResourceHandle<Font> fontHandle)
			: textID{ std::move(id) }, fontHandle{ fontHandle }
		{
		}

		std::string textID = "";

		std::string text = "";
		ResourceHandle<Texture> textureHandle; // Test...
		ResourceHandle<Font> fontHandle; // Store handles?? or just raw data (id, font type, size)? handles invalidated -> but genrated again in text system
		Color color = Color::black;
		int fontSize = 12;
		bool isDirty = true; // false;

		// could store a Texture here.... 
	};

	struct AudioComponent
	{
		ResourceHandle<Audio> audioHandle;
		bool isLooping;
	};
}