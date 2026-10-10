#include "engine/modules/ecs_module.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/component/component_registry.h"

#include "engine/core/result.h"

#include "engine/utils/json/json_value.h"
#include "engine/resources/font/font.h"
#include "engine/rendering/text/text_creator.h"
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

	static const std::unordered_map<std::string, Shape::Type> shapeTypes
	{
		{ "square", Shape::Type::Square },
		{ "rectangle", Shape::Type::Rectangle },
		{ "circle", Shape::Type::Circle }
	};

	static Result<Shape> parseShape(const JsonValue& value)
	{
		const std::string shapeType = value["type"].asString();

		if (shapeType == "square")
		{
			return Result<Shape>::success(Shape::square(value["half_extent"].asFloat()));
		}
		else if (shapeType == "rectangle")
		{
			return Result<Shape>::success(Shape::rectangle(value["width"].asFloat(), value["height"].asFloat()));
		}
		else if (shapeType == "circle")
		{
			return Result<Shape>::success(Shape::circle(value["radius"].asFloat()));
		}
	}

	static Result<Shape> parseShape(const PropertyValue& value)
	{
		const ComponentProperties& shapeProperties = value.get<ComponentProperties>();

		const PropertyValue& typeValue = shapeProperties.at("type");
		const std::string& shapeType = typeValue.get<std::string>();

		if (shapeType == "square")
		{
			return Result<Shape>::success(Shape::square(shapeProperties.at("half_extent").get<float>()));
		}
		else if (shapeType == "rectangle")
		{
			return Result<Shape>::success(Shape::rectangle(
				shapeProperties.at("width").get<float>(),
				shapeProperties.at("height").get<float>())
			);
		}
		else if (shapeType == "circle")
		{
			return Result<Shape>::success(Shape::circle(shapeProperties.at("radius").get<float>()));
		}

		return Result<Shape>::failure("Not a recognized shape!");
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
		registerComponent<AnimationComponent>(m_componentRegistry, "animation",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				std::string animationSetId = std::get<std::string>(properties.at("animation_set_id"));
				AssetHandle assetHandle = ctx.assetManager->getAssetHandle<AnimationSet>(std::move(animationSetId));

				std::string currentAnimationId = std::get<std::string>(properties.at("active_animation_id"));

				handle.attachComponent<AnimationComponent>(std::move(assetHandle), std::move(currentAnimationId));
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string animationSetId = value["animation_set_id"].asString();
				AssetHandle assetHandle = ctx.assetManager->getAssetHandle<AnimationSet>(std::move(animationSetId));

				std::string currentAnimationId = value["active_animation_id"].asString();

				handle.attachComponent<AnimationComponent>(std::move(assetHandle), std::move(currentAnimationId));
			});

		registerComponent<AudioComponent>(m_componentRegistry, "audio",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				assert(false && "Not implemented!");
				//handle.attachComponent<AudioComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
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

		registerComponent<BoundingBoxComponent>(m_componentRegistry, "bounding_box",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{

			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				float xOffset = value["offset"]["x"].asFloat();
				float yOffset = value["offset"]["y"].asFloat();

				float width = value["size"]["width"].asFloat();
				float height = value["size"]["height"].asFloat();

				handle.attachComponent<BoundingBoxComponent>(FVec2{ xOffset, yOffset }, FVec2{ width, height });
			});

		registerComponent<ButtonComponent>(m_componentRegistry, "button",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
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

		registerComponent<CameraComponent>(m_componentRegistry, "camera",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				handle.attachComponent<CameraComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				// TODO; get window size fro msettings class?
				handle.attachComponent<CameraComponent>(IVec2{ 1280, 720 });
			});

		registerComponent<CheckboxComponent>(m_componentRegistry, "checkbox",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string checkedRegionId = value["checked_region"].asString();
				std::string uncheckedRegionId = value["unchecked_region"].asString();

				bool isChecked = value["is_checked"].asBool();
				handle.attachComponent<CheckboxComponent>(checkedRegionId, uncheckedRegionId, isChecked);
			},
			[](EntityHandle& handle, const ComponentPostInitContext& ctx)
			{
				const SpriteComponent& spriteComponent = handle.getComponent<SpriteComponent>();
				const TextureAtlas& atlas = ctx.assetManager->getAsset<TextureAtlas>(spriteComponent.atlasHandle);

				CheckboxComponent& checkboxComponent = handle.getComponent<CheckboxComponent>();

				std::size_t checkedRegionIndex = atlas.idToRegionIndex.at(checkboxComponent.checkedRegionId);
				std::size_t uncheckedRegionIndex = atlas.idToRegionIndex.at(checkboxComponent.uncheckedRegionId);

				checkboxComponent.checkedRegion = atlas.regions.at(checkedRegionIndex);
				checkboxComponent.uncheckedRegion = atlas.regions.at(uncheckedRegionIndex);
			});

		registerComponent<FollowComponent>(m_componentRegistry, "follow",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				handle.attachComponent<FollowComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<FollowComponent>();
			});

		// remove either hiearchy or parent compoentn! store offset not in compoennt, but in paretn? /bfe
		registerComponent<HierarchyComponent>(m_componentRegistry, "hierarchy",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
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

		/*registerComponent<InputComponent>(registry, "input",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{},
			[](EntityHandle& handle, const JsonValue& value)
			{
			});*/

		registerComponent<LayoutComponent>(m_componentRegistry, "layout",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				handle.attachComponent<LayoutComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<LayoutComponent>();
			});

		//registerComponent<ParentComponent>("parent",
		//	[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)
		//	{
		//	},
		//	[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx) 
		//	{
		//		handle.attachComponent<ParentComponent>(value["parent_id"].asString());s
		//		//how to  find parent? -> send event "Entity Created"? let systme handle it?
		//	});

		registerComponent<PhysicsComponent>(m_componentRegistry, "physics",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				Result<ColliderType> colliderTypeResult = parseColliderType(properties.at("collider_type").get<std::string>());

				if (!colliderTypeResult.ok())
				{
					// LOG error?
					return;
				}

				BodyDefinition bodyDefinition;
				bodyDefinition.type = colliderTypeResult.take();

				Result<Shape> shapeResult = parseShape(properties.at("shape"));

				if (!shapeResult.ok())
				{
					return;
				}

				bodyDefinition.shape = shapeResult.take();

				bodyDefinition.linearDamping = 0.0f;
				bodyDefinition.angularDamping = 0.0f;

				handle.attachComponent<PhysicsComponent>(std::move(bodyDefinition));
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
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

		registerComponent<SliderComponent>(m_componentRegistry, "slider",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				handle.attachComponent<SliderComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<SliderComponent>();
			});

		registerComponent<SpriteComponent>(m_componentRegistry, "sprite",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				std::string id = std::get<std::string>(properties.at("texture_id"));

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

				handle.attachComponent<SpriteComponent>(atlasHandle, region, color);
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

				handle.attachComponent<SpriteComponent>(atlasHandle, region, color);
			});

		registerComponent<TextComponent>(m_componentRegistry, "text",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{

			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
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

				if (!fontHandle.isValid())
				{
					//Logger::logError();
					return;
				}

				const auto& font = ctx.fontManager->get(fontHandle);

				Result<TextPtr> result = ctx.textCreator->createText(ctx.localization->getText(textId), font);

				if (!result.ok())
				{
					return; // LOG? or return result 
				}

				if (auto text = result.take())
				{
					Color textColor = Color::black;

					if (value.hasMember("color"))
					{
						textColor.r = value["color"]["r"].asInt();
						textColor.g = value["color"]["g"].asInt();
						textColor.b = value["color"]["b"].asInt();
						textColor.a = value["color"]["a"].asInt();
					}

					text->setTextColor(textColor); // TODO: do in factory? or use abuilder....

					FVec2 pivot{};
					if (value.hasMember("pivot"))
					{
						pivot.x = value["pivot"]["x"].asFloat();
						pivot.y = value["pivot"]["y"].asFloat();
					}

					handle.attachComponent<TextComponent>(std::move(text), pivot); // Only if suceesful??
					//handle.attachComponent<TextComponent>(textId, fontHandle, std::move(text), pivot, textColor); // Only if suceesful??
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

		registerComponent<TransformComponent>(m_componentRegistry, "transform",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				using PropertyMap = std::unordered_map<std::string, PropertyValue>;

				FVec2 position{};

				if (auto it = properties.find("position"); it != properties.end())
				{
					const PropertyMap& positionProperty = std::get<PropertyMap>(it->second);

					position.x = std::get<float>(positionProperty.at("x"));
					position.y = std::get<float>(positionProperty.at("y"));
				}
				else
				{
					position = data.position;
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

		registerComponent<UIComponent>(m_componentRegistry, "ui",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				handle.attachComponent<UIComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				handle.attachComponent<UIComponent>();
			});

		registerComponent<VelocityComponent>(m_componentRegistry, "velocity",
			[](EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx, const SpawnData& data)
			{
				handle.attachComponent<VelocityComponent>();
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				/// assert attachcompoent type is same as registercomponen ttype...
				handle.attachComponent<VelocityComponent>();
			});
	}
}