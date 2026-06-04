#include "engine/modules/ecs_module.h"
#include "engine/ecs/component/core_components.h"
#include "engine/ecs/component/component_registry.h"
#include "engine/ecs/system/render_system.h",
#include "engine/ecs/system/interaction_system.h"
#include "engine/ecs/system/input_system.h"
#include "engine/ecs/system/ui_system.h"
#include "engine/ecs/system/text_system.h"
#include "engine/ecs/system/audio_system.h"
#include "engine/utils/json/json_value.h"
#include "engine/resources/resource_types.h"
#include "engine/resources/text/font.h"

#include "engine/core/engine_context.h"

#include "engine/core/localization/localization.h"
#include "engine/core/logger.h"
#include "engine/platform/input/input_handler.h"
#include "engine/resources/text/text_factory.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/render_types.h"
#include "engine/audio/audio_controller.h"

#include "engine/resources/resource_types.h"

namespace cursed_engine
{
	bool ECSModule::init(const EngineContext& context)
	{
		m_entityFactory.init(context.assets.assetManager);

		registerCoreComponents();
		insertCoreSystems(context);

		Logger::logInfo("-> ECSModule: Success");
		return true;
	}

	void ECSModule::shutdown()
	{
	}

	/*ECSServices ECSModule::getServices() noexcept
	{
		return { &m_entityFactory, &m_componentRegistry, &m_systemManager };
	}*/

	void ECSModule::registerCoreComponents()
	{
		// TODO; use config to check which subsystems are active, only register if active? (physics -> physicsComponent)

		m_componentRegistry.registerComponent<TransformComponent>("transform",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
				auto& transformComponent = handle.getComponent<TransformComponent>();
				//transformComponent.position = properties.at("position"); // WORKS???
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				float x = (float)value["x"].asDouble();
				float y = (float)value["y"].asDouble();

				// TODO; also valid check it contains the data?

				float width = (float)value["width"].asDouble();
				float height = (float)value["height"].asDouble();

				FVec2 pivot{ 0.f, 0.f };

				if (value.has("pivot")) {
					pivot.x = (float)value["pivot"]["x"].asDouble();
					pivot.y = (float)value["pivot"]["y"].asDouble();
				}

				float rotation = 0.f;

				if (value.has("rotation"))
					float rotation = (float)value["rotation"].asDouble();

				handle.attachComponent<TransformComponent>(FVec2{ x, y }, FVec2{ width, height }, pivot, rotation);
			});

		m_componentRegistry.registerComponent<SpriteComponent>("sprite",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string id = value["id"].asString();

				// [[consider]] if better to store only id at this point, and not reference any manager?
				//const auto atlasHandle = context.asset.assetManager.getAssetHandle<TextureAtlas>(id); // pass in id?
				const auto atlasHandle = ctx.assetManager->getAssetHandle<TextureAtlas>(id); // pass in id?

				if (!atlasHandle.isValid())
				{
					Logger::logError("Missing texture atlas with id: " + id);
					// TODO; attach error texture!
				}

				AtlasRegion region; // TODO; fix!
				region.x = 0;
				region.y = 0;
				region.w = 700;
				region.h = 700;

				//std::array<float, 4> color{ 1.f, 1.f, 1.f, 1.f };

				Color color = Color::white;

				if (value.has("color"))
				{
					color.r = value["color"]["r"].asInt();
					color.g = value["color"]["g"].asInt();
					color.b = value["color"]["b"].asInt();

					if (value["color"].has("a"))
						color.a = value["color"]["a"].asInt();
				}

				float zOrder = 1.f;

				// Figue out...
				// Treat background images and texture alias the same? or solve with union/variant?


				/*
					AssetHandle atlasHandle; // Handle to texture atlas
					AtlasRegion region;
					float colors[4];
					float zOrder;
				*/

				handle.attachComponent<SpriteComponent>(atlasHandle, region, color, zOrder);
			});


		/*registerComponent<InputComponent>(registry, "input",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{},
			[](EntityHandle& handle, const JsonValue& value)
			{
			});*/

			// TODO; interaction component instead???
		m_componentRegistry.registerComponent<ButtonComponent>("button",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{
			},
			[](EntityHandle& handle, const JsonValue& value, const ComponentInitContext& ctx)
			{
				std::string action = value["action"].asString();

				// TODO; get params
				ActionArgs args;
				if (value.has("args"))
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

				if (value.has("default_color"))
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

				if (value.has("hover_color"))
				{
					optHoverColor = Color{};

					auto& hoverColor = optHoverColor.value();
					hoverColor.r = value["hover_color"]["r"].asInt();
					hoverColor.g = value["hover_color"]["g"].asInt();
					hoverColor.b = value["hover_color"]["b"].asInt();
					hoverColor.a = value["hover_color"]["a"].asInt();
				}

				std::optional<Color> optPressedColor = std::nullopt;

				if (value.has("pressed_color"))
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


		m_componentRegistry.registerComponent<BoundingBoxComponent>("bounding_box",
			[](EntityHandle& handle, const ComponentProperties& properties)
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

		m_componentRegistry.registerComponent<TextComponent>("text",
			[](EntityHandle& handle, const ComponentProperties& properties)
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
				if (value.has("font_style"))
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
				int outline = value.has("outline") ? value["outline"].asInt() : 0;

				//bool kerning
				bool kerning = value.has("kerning") ? value["kerning"].asBool() : true;

				std::string textId = value["text_id"].asString();

				// handle in system?
				auto fontHandle = ctx.fontManager->getHandleById(fontType, fontStyle, fontSize, outline, kerning);
				auto textObj = ctx.textFactory->createText(ctx.localization->getText(textId), fontHandle);

				{
					// TEST
					//auto* font = engineResources.fontManager.get(fontHandle);
					//font.set
				}

				if (fontHandle.isValid())
				{
					Color textColor = Color::black;

					if (value.has("color"))
					{
						textColor.r = value["color"]["r"].asInt();
						textColor.g = value["color"]["g"].asInt();
						textColor.b = value["color"]["b"].asInt();
						textColor.a = value["color"]["a"].asInt();
					}

					textObj.setTextColor(textColor); // TODO: do in factory? or use abuilder....

					handle.attachComponent<TextComponent>(textId, fontHandle, std::move(textObj), textColor); // Only if suceesful??
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

		m_componentRegistry.registerComponent<AudioComponent>("audio",
			[](EntityHandle& handle, const ComponentProperties& properties)
			{},
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
	}

	void ECSModule::insertCoreSystems(const EngineContext& context)
	{
		auto& rendering = context.rendering;
		auto& platform = context.platform;
		auto& assets = context.assets;
		auto& resources = context.resources;

		m_systemManager.emplace<RenderSystem>(resources.textureManager, assets.assetManager, rendering.rendererAPI);
		//m_systemManager.emplace<InputSystem>(inputHandler);
		m_systemManager.emplace<InteractionSystem>();
		m_systemManager.emplace<UISystem>(platform.inputHandler, context.actionRegistry); // OR Accept action registry (and event bus) by pointer?
		m_systemManager.emplace<TextSystem>(resources.textManager, resources.textFactory, assets.localization);
		m_systemManager.emplace<AudioSystem>(resources.audioManager, context.audio.audioController, context.eventBus); // FIX eventbus ptr
	}
}