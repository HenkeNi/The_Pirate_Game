#pragma region
#include "engine/math/vec2.hpp"
#include "engine/assets/asset_types.h"
#include "engine/assets/asset_manager.h" // remove include?
#include "engine/rendering/animation_types.h"
#include "engine/core/action/action_registry.h" // or put type alisas in action.h?
#include "engine/ecs/entity/entity_handle.h"
#include "engine/rendering/render_types.h"
#include "engine/resources/text/text.h"
#include <optional>
#include <array>
#include <unordered_map>

namespace cursed_engine
{
	/*class ComponentRegistry; 
	class EngineResources;
	class Localization;
	struct ResourceConfig;
*/

	// Separet ui transform and world transform? or screen space / world space?

	//struct TransformComponent
	//{
	//	// TODO; add 2 constructors? one for each individual argument?

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

	struct TransformComponent
	{
		// TODO; add 2 constructors? one for each individual argument?

		FVec2 position{ 0.f, 0.f };
		FVec2 scale{ 1.f, 1.f };
		FVec2 pivot{ 0.f, 0.f }; // helper functions or add funct in struct?
		float rotation = 0.f;


		float zIndex = 0.f;
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
		/*
		* struct SpriteComponent
			{
				TextureHandle texture;
				Rect sourceRect;

				Vector2 pivot;
			};
		*/

		/*
		* struct SpriteComponent
		* {
		*	TextureHandle texture;
		* vec2 uvTopLeft;
		* vec2 uvBottomRight;
		* };
		*/

		//SpriteComponent(AtlasH handle, AtlasRegion region, float color[4], float z)
		//	: atlasH

		// wrap in sprite struct?
		AssetHandle atlasHandle; // Handle to texture                   --- better than storing id to atlas handle? (maybe to much indirection)
		AtlasRegion atlasRegion;

		Color color = Color::white;
		
		//std::array<float, 4> color; // use COlor instead?
		//float colors[4]; // create type or type alias for color!

		// std::variant<TextureHandle, AtlasSpriteHandle> source;

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

	struct BoundingBoxComponent
	{
		FVec2 offset;
		FVec2 halfSize; // figure out a good way where you dont have to specify bounding box size in json (if change transform size, you need to remebe to update bounding box as well)

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

	//struct HierarchyComponent // or name ParentComponent
	//{
	//	Entity parent;
	//	// also children?
	//};

	struct ParentComponent
	{
		ParentComponent() = default;
		
		ParentComponent(std::string parentIdentifier)
			: parentIdentifier{ std::move(parentIdentifier) }
		{
		}

		ParentComponent(EntityHandle parent, std::string parentIdentifier)
			: parent{ std::move(parent) }, parentIdentifier{ std::move(parentIdentifier) }
		{
		}

		EntityHandle parent; // or just entity?
		std::string parentIdentifier;
	};

	//struct ButtonAction
	//{
	//	// onHover
	//	// onPress or onClick
	//};


	// interactable?
	struct ButtonComponent
	{
		//using ActionValue = std::variant<std::string, int, double, bool>;

		ButtonComponent() = default;
		ButtonComponent(std::string action, ActionArgs args, Color defaultColor, std::optional<Color> hoverColor = std::nullopt, std::optional<Color> pressedColor = std::nullopt)
			: action{ action }, actionArgs{ std::move(args) }, defaultColor{defaultColor}, hoverColor{hoverColor}, pressedColor{pressedColor}
		{
		}

		enum class State{ Normal, Hovered, Pressed }; // replace with input state instead?
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
		TextComponent(std::string id, ResourceHandle<Font> fontHandle, Text text, Color color = Color::black)
			: textID{ std::move(id) }, fontHandle{ fontHandle }, textObj{ std::move(text) }, color{ color }
		{
		}

		std::string textID = "";

		std::string text = "";
		ResourceHandle<Texture> textureHandle; // Test...
		ResourceHandle<Font> fontHandle; // Store handles?? or just raw data (id, font type, size)? handles invalidated -> but genrated again in text system
		Color color = Color::black; // store here??? or always have a sprite component to texts?
		int fontSize = 12;
		bool isDirty = true; // false;


		Text textObj;

		// could store a Texture here.... 
	};

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

	// Tab? View? Tooltip? ProgressBar?
}