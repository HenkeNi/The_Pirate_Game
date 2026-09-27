#include "engine/modules/ecs_module.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/component/component_registry.h"

#include "engine/core/result.h"

#include "engine/utils/json/json_value.h"
#include "engine/resources/resource_types.h" // res managers
#include "engine/resources/text/font.h"
#include "engine/resources/text/text_manager.h"
#include "engine/rendering/render_types.h"

#include "engine/physics/physics.h"

#include "engine/core/engine_context.h"
#include "engine/core/localization/localization.h"
#include "engine/core/logger.h"
//#include "engine/platform/input/input.h"
#include "engine/audio/audio_controller.h"
#include <format>
#include <string>
#include <unordered_map>
// TODO; handle missing values in json (registerComponents)

namespace cursed_engine
{
#pragma region Helpers

	static Result<ColliderType> parseColliderType(const std::string& type)
	{
		static const std::unordered_map<std::string, ColliderType> colliderTypes
		{
			{ "dynamic", ColliderType::Dynamic },
			{ "kinematic", ColliderType::Kinematic },
			{ "static", ColliderType::Static }
		};

		if (auto it = colliderTypes.find(type); it != colliderTypes.end())
		{
			return Result<ColliderType>::success(it->second);
		}

		return Result<ColliderType>::failure(std::format("Unknown collider type found: {}", type));
	}

	static Result<Shape> parseShape(const JsonValue& value)
	{
		static const std::unordered_map<std::string, Shape::ShapeType> shapeTypes
		{
			{ "square", Shape::ShapeType::Square },
			{ "rectangle", Shape::ShapeType::Rectangle },
			{ "circle", Shape::ShapeType::Circle }
		};

		const std::string shapeType = value["type"].asString();

		auto it = shapeTypes.find(shapeType);
		if (it == shapeTypes.end())
		{
			return Result<Shape>::failure(std::format("Unknown shape type found: {}", shapeType));
		}

		Shape shape;
		shape.type = it->second;

		switch (shape.type)
		{
		case Shape::ShapeType::Square:
			shape.data.Square.halfExtent = value["half_extent"].asFloat();
			break;
		case Shape::ShapeType::Rectangle:
			shape.data.Rectangle.width = value["width"].asFloat(); 
			shape.data.Rectangle.height = value["height"].asFloat();
			break;
		case Shape::ShapeType::Circle:
			shape.data.Circle.radius = value["radius"].asFloat();
			break;
		}

		return Result<Shape>::success(shape);
	}

#pragma endregion

	ECSModule::ECSModule()
		: m_entityFactory{ m_componentRegistry }
	{
	}

	bool ECSModule::init(const EngineContext& context)
	{
		Logger::logInfo(std::format("{}[ECSModule] - Initialization started...", log_format::INDENT));

		m_entityFactory.init(context.assets.assetManager);

		registerCoreComponents();

		Logger::logInfo(std::format("{}[ECSModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void ECSModule::shutdown()
	{
		m_componentRegistry.clear();
		m_systemManager.clear();
	}

	void ECSModule::registerCoreComponents()
	{
		// TODO; use config to check which subsystems are active, only register if active? (physics -> physicsComponent)

		registerComponent<TransformComponent>(m_componentRegistry, "transform",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				using PropertyMap = std::unordered_map<std::string, PropertyValue>;

				FVec2 position{};

				if (auto it = properties.find("position"); it != properties.end())
				{
					const PropertyMap& positionProperty = std::get<PropertyMap>(it->second);

					position.x = std::get<float>(positionProperty.at("x"));
					position.y = std::get<float>(positionProperty.at("y"));
				}

				FVec2 scale{};

				if (auto it = properties.find("scale"); it != properties.end())
				{
					const PropertyMap& scaleProperty = std::get<PropertyMap>(it->second);

					scale.x = std::get<float>(scaleProperty.at("x"));
					scale.y = std::get<float>(scaleProperty.at("y"));
				}

				float rotation = 0;

				if (auto it = properties.find("rotation"); it != properties.end())
				{
					rotation = std::get<float>(it->second);
				}

				handle.attachComponent<TransformComponent>(position, scale, rotation);
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				FVec2 position{};

				if (value.hasMember("position"))
				{
					assert(value["position"].isObject() && "position must be an object!");

					const JsonValue positionValue = value["position"];

					position.x = positionValue["x"].asFloat();
					position.y = positionValue["y"].asFloat();
				}

				FVec2 scale{};

				if (value.hasMember("scale"))
				{
					assert(value["scale"].isObject() && "scale must be an object!");

					const JsonValue scaleValue = value["scale"];

					scale.x = scaleValue["x"].asFloat();
					scale.y = scaleValue["y"].asFloat();
				}

				float rotation = 0.f;

				if (value.hasMember("rotation"))
				{
					rotation = value["rotation"].asFloat();
				}

				handle.attachComponent<TransformComponent>(position, scale, rotation);
			});

		registerComponent<CameraComponent>(m_componentRegistry, "camera",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<CameraComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				// TODO; get window size fro msettings class?
				handle.attachComponent<CameraComponent>(IVec2{ 1280, 720 });
			});

		registerComponent<VelocityComponent>(m_componentRegistry, "velocity",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<VelocityComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				/// assert attachcompoent type is same as registercomponen ttype...
				handle.attachComponent<VelocityComponent>();
			});

		registerComponent<SpriteComponent>(m_componentRegistry, "sprite",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				std::string id = std::get<std::string>(properties.at("id"));

				const AssetHandle atlasHandle = ctx.assetManager->getAssetHandle<TextureAtlas>(id); // pass in id?

				if (!atlasHandle.isValid())
				{
					Logger::logError("Missing texture atlas with id: " + id);
					// todo, attach error texture!
				}


				assert(properties.contains("region") && "[Prefab Instantiation] - Invalid property format: SpriteComponent");

				std::string regionId = std::get<std::string>(properties.at("region"));
				//const AssetHandle handle = ctx.assetManager->getAssetHandle<TextureAtlas>(std::move(region));

				const TextureAtlas& atlas = ctx.assetManager->getAsset<TextureAtlas>(atlasHandle);


				std::size_t regionIndex = atlas.idToRegionIndex.at(regionId);
				AtlasRegion region = atlas.regions[regionIndex];


				Color color = Color::white;

				if (properties.contains("color"))
					int x = 20;

				if (properties.contains("r"))
				{
					int y = 20;
					/*color.r = value["color"]["r"].asInt();
					color.g = value["color"]["g"].asInt();
					color.b = value["color"]["b"].asInt();

					if (value["color"].hasMember("a"))
						color.a = value["color"]["a"].asInt();*/
				}

				float zOrder = 1.f;

				handle.attachComponent<SpriteComponent>(atlasHandle, region, color, zOrder);
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{

				std::string id = value["texture_id"].asString();

				// [[consider]] if better to store only id at this point, and not reference any manager?
				//const auto atlasHandle = context.asset.assetManager.getAssetHandle<TextureAtlas>(id); // pass in id?
				const AssetHandle atlasHandle = ctx.assetManager->getAssetHandle<TextureAtlas>(id); // pass in id?

				if (!atlasHandle.isValid())
				{
					Logger::logError("Missing texture atlas with id: " + id);
					// TODO; attach error texture!
				}

				const TextureAtlas& atlas = ctx.assetManager->getAsset<TextureAtlas>(atlasHandle);

				AtlasRegion region;

				if (value.hasMember("region"))
				{
					std::string regionId = value["region"].asString();
					std::size_t index = atlas.idToRegionIndex.at(regionId);

					region = atlas.regions.at(index);
				}
				else
				{
					// TODO; think of a better idea than loading in the resource? size in json? optional atlas region?

					

					// load in resource just to set? make region optional?
					//IVec2 textureSize = atlas.textureSize; // THIS IS NOT SET! 
					region.rect.x = 0;
					region.rect.y = 0;
					region.rect.w = 1280; //textureSize.x;
					region.rect.h = 720; //textureSize.y;
				}

				// Figue out...
				// Treat background images and texture alias the same? or solve with union/variant?

				Color color = Color::white;

				if (value.hasMember("color"))
				{
					assert(value["color"].isObject() && "Color is wrongly formatted in json!");

					color.r = value["color"]["r"].asInt();
					color.g = value["color"]["g"].asInt();
					color.b = value["color"]["b"].asInt();

					if (value["color"].hasMember("a"))
						color.a = value["color"]["a"].asInt();
				}

				float zOrder = 1.f;

				handle.attachComponent<SpriteComponent>(atlasHandle, region, color, zOrder);
			});

		registerComponent<AnimationComponent>(m_componentRegistry, "animation",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				assert(false && "No properties are set!");
				//handle.attachComponent<AnimationComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string animationSetId = value["animation_set_id"].asString();
				AssetHandle assetHandle = ctx.assetManager->getAssetHandle<AnimationSet>(std::move(animationSetId));

				std::string currentAnimationId = value["active_animation_id"].asString();

				handle.attachComponent<AnimationComponent>(std::move(assetHandle), std::move(currentAnimationId));
			});

		registerComponent<UIComponent>(m_componentRegistry, "ui",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<UIComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<UIComponent>();
			});

		registerComponent<LayoutComponent>(m_componentRegistry, "layout",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<LayoutComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<LayoutComponent>();
			});

		/*registerComponent<InputComponent>(registry, "input",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{},
			[](EntityHandle& handle, const JsonValue& value)
			{
			});*/

			// TODO; interaction component instead???
		registerComponent<ButtonComponent>(m_componentRegistry, "button",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<ButtonComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string action = value["action"].asString();

				// TODO; get params
				ActionArgs args;
				if (value.hasMember("args"))
				{
					value["args"].forEachProperty(
						[&](std::string name, JsonValue value)
						{
							if (value.isInt())
								args.insert_or_assign(name, value.asInt());
							else if (value.isString())
								args.insert_or_assign(name, value.asString());
							else if (value.isDouble())
								args.insert_or_assign(name, value.asDouble());
							else if (value.isBool())
								args.insert_or_assign(name, value.asBool());
							else
							{
								Logger::logError("Failed to parse (Action) value from json");
								assert(false && "Failed to parse");
							}
						});
				}

				//std::string val = value["value"].asString(); // asAny?

				Color defaultColor;

				if (value.hasMember("default_color"))
				{
					defaultColor.r = value["default_color"]["r"].asInt();
					defaultColor.g = value["default_color"]["g"].asInt();
					defaultColor.b = value["default_color"]["b"].asInt();
					defaultColor.a = value["default_color"]["a"].asInt();
				}
				else
				{
					defaultColor = Color::white;
					Logger::logWarning("Button is missing default color, using default");
				}

				std::optional<Color> optHoverColor = std::nullopt;

				if (value.hasMember("hover_color"))
				{
					optHoverColor = Color{};

					auto& hoverColor = optHoverColor.value();
					hoverColor.r = value["hover_color"]["r"].asInt();
					hoverColor.g = value["hover_color"]["g"].asInt();
					hoverColor.b = value["hover_color"]["b"].asInt();
					hoverColor.a = value["hover_color"]["a"].asInt();
				}

				std::optional<Color> optPressedColor = std::nullopt;

				if (value.hasMember("pressed_color"))
				{
					optPressedColor = Color{};

					auto& pressedColor = optPressedColor.value();
					pressedColor.r = value["pressed_color"]["r"].asInt();
					pressedColor.g = value["pressed_color"]["g"].asInt();
					pressedColor.b = value["pressed_color"]["b"].asInt();
					pressedColor.a = value["pressed_color"]["a"].asInt();
				}

				handle.attachComponent<ButtonComponent>(action, std::move(args), defaultColor, optHoverColor, optPressedColor);
			});


		registerComponent<BoundingBoxComponent>(m_componentRegistry, "bounding_box",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{

			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				int xOffset = value["offset"]["x"].asInt();
				int yOffset = value["offset"]["y"].asDouble();

				int width = value["size"]["width"].asInt() * 0.5f; // TODO; use halfextent instead!
				int height = value["size"]["height"].asInt() * 0.5f; // TODO; decide if float or int...

				handle.attachComponent<BoundingBoxComponent>(FVec2{ (float)xOffset, (float)yOffset }, FVec2{ (float)width, (float)height });
			});

		registerComponent<TextComponent>(m_componentRegistry, "text",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{

			},
			[&](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string fontType = value["font"].asString();
				int fontSize = value["size"].asInt();

				// maybe just store id?

				//const auto& resourceIdToPath = resourceConfig.resourceIdToPath;

				static const std::unordered_map<std::string, FontStyle> fontStyles =
				{
					{ "Normal", FontStyle::Normal },
					{ "Bold", FontStyle::Bold },
					{ "Italic", FontStyle::Italic },
					{ "Underline", FontStyle::Underline },
					{ "Strikethrough", FontStyle::Strikethrough },
				};


				FontStyle fontStyle = FontStyle::Normal;
				if (value.hasMember("font_style"))
				{
					std::string style = value["font_style"].asString();

					if (auto it = fontStyles.find(style); it != fontStyles.end())
					{
						fontStyle = it->second;
					}
					else
					{
						Logger::logWarning("Unknown font specified in JSON: " + it->first);
					}
				}


				// style.. 
				//int size;

				//int outline;
				int outline = value.hasMember("outline") ? value["outline"].asInt() : 0;

				//bool kerning
				bool kerning = value.hasMember("kerning") ? value["kerning"].asBool() : true;

				std::string textId = value["text_id"].asString();

				// handle in system?
				auto fontHandle = ctx.fontManager->getHandleById(fontType, fontStyle, fontSize, outline, kerning);
				auto textObj = ctx.textManager->createText(ctx.localization->getText(textId), fontHandle); // ctx.textFactory->createText(ctx.localization->getText(textId), fontHandle);

				{
					// TEST
					//auto* font = engineResources.fontManager.get(fontHandle);
					//font.set
				}

				if (fontHandle.isValid())
				{
					Color textColor = Color::black;

					if (value.hasMember("color"))
					{
						textColor.r = value["color"]["r"].asInt();
						textColor.g = value["color"]["g"].asInt();
						textColor.b = value["color"]["b"].asInt();
						textColor.a = value["color"]["a"].asInt();
					}

					textObj.setTextColor(textColor); // TODO: do in factory? or use abuilder....

					FVec2 pivot{};
					if (value.hasMember("pivot"))
					{
						pivot.x = value["pivot"]["x"].asFloat();
						pivot.y = value["pivot"]["y"].asFloat();
					}

					handle.attachComponent<TextComponent>(std::move(textObj), pivot); // Only if suceesful??
					//handle.attachComponent<TextComponent>(textId, fontHandle, std::move(textObj), pivot, textColor); // Only if suceesful??
				}
				else
				{
					Logger::logWarning(std::format("[Constructing TextComponent from Json] - Failed to find path for font with id: {}", textId));
					assert(false && "Invalid font handle!");
				}


				//if (auto it = resourceIdToPath.find(fontType); it != resourceIdToPath.end())
				//{
				//	auto fontHandle = engineResources.fontManager.getHandle({ it->second, fontSize }); // safe?

				//	if (!fontHandle.isValid())
				//	{
				//		int x = 20;
				//	}

				//	handle.attachComponent<TextComponent>(id, fontHandle); // Only if suceesful??
				//}
				//else
				//{
				//	Logger::logWarning(std::format("[Constructing TextComponent from Json] - Failed to find path for font with id: {}", id));
				//	// Dont attach component? or use some debug font?
				//}

				//localization.getText(id);

				//auto handle = engineResources.getHandle<Texture>(id);


				// store lockup table id to text (and perhaps text to id)

				//auto handle = engineResources.getHandle<Texture>(id); // WIll this load the resource? probably not

				// safe to pass localization? OR handle in TextSystem?
			});

		registerComponent<AudioComponent>(m_componentRegistry, "audio",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<AudioComponent>();
			},
			[&](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string sound = value["sound"].asString();
				auto audioHandle = ctx.audioManager->getHandleById(std::move(sound));

				if (!audioHandle.isValid())
				{

				}

				bool isLooping = value["should_loop"].asBool();

				// TODO; always check if contains? if no sound fail? if no is looping, default to false?

				handle.attachComponent<AudioComponent>(audioHandle, isLooping);
			});

		registerComponent<CheckboxComponent>(m_componentRegistry, "checkbox",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
			},
			[&](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				bool isChecked = value["is_checked"].asBool();
				handle.attachComponent<CheckboxComponent>(isChecked);

			});

		registerComponent<SliderComponent>(m_componentRegistry, "slider",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<SliderComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<SliderComponent>();
			});

		//m_componentRegistry.registerComponent<ParentComponent>("parent",
		//	[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		//	{
		//	},
		//	[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx) 
		//	{
		//		handle.attachComponent<ParentComponent>(value["parent_id"].asString());

		//		//how to  find parent? -> send event "Entity Created"? let systme handle it?
		//	});

		// remove either hiearchy or parent compoentn! store offset not in compoennt, but in paretn? /bfe
		registerComponent<HierarchyComponent>(m_componentRegistry, "hierarchy",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<HierarchyComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				FVec2 offset{};

				if (value.hasMember("offset"))
				{
					assert(value.isObject() && "Offset is wrongly formated");

					const JsonValue offsetValue = value["offset"];

					offset.x = offsetValue["x"].asFloat();
					offset.y = offsetValue["y"].asFloat();
				}

				handle.attachComponent<HierarchyComponent>(offset);
			});

		registerComponent<FollowComponent>(m_componentRegistry, "follow",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<FollowComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<FollowComponent>();
			});

		registerComponent<PhysicsComponent>(m_componentRegistry, "physics",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
			{
				handle.attachComponent<PhysicsComponent>();
			},
			[&](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				BodyDefinition bodyDefinition;

				Result<ColliderType> colliderTypeResult = parseColliderType(value["collider_type"].asString());

				if (!colliderTypeResult.ok())
				{
					// LOG error?
					return;
				}

				bodyDefinition.type = colliderTypeResult.take();
				
				Result<Shape> shapeResult = parseShape(value["shape"]);
				
				if (!shapeResult.ok())
				{
					return;
				}

				bodyDefinition.shape = shapeResult.take();

				bodyDefinition.linearDamping = 0.0f;
				bodyDefinition.angularDamping = 0.0f;
		
				handle.attachComponent<PhysicsComponent>(std::move(bodyDefinition));
			},
			[](EntityHandle& handle, const ComponentPostInitContext& ctx)
			{
				assert(ctx.physicsWorld && "Invalid PhysicsWorld");

				PhysicsComponent& physicsComponent = handle.getComponent<PhysicsComponent>();				
				TransformComponent& transformComponent = handle.getComponent<TransformComponent>(); // Is this the best approach?

				// either store collider type, shae, etc directly in physics component or store a "BOdy" and then call body init to actually create?

				physicsComponent.bodyDefinition.position = transformComponent.position;
				physicsComponent.bodyDefinition.rotation = transformComponent.rotation;

				// DONT use position directly! -> use pivot to? or just center body? (center is middle)

				PhysicsBody body = ctx.physicsWorld->createBody(physicsComponent.bodyDefinition); // This requires body definition data to be stored in component...
				physicsComponent.physicsBody = std::move(body);
				// int x = 20;
				//ctx.physicsWorld->createBody();
			});
	}
}