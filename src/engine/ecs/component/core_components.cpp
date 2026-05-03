#include "engine/ecs/component/core_components.h"
#include "engine/ecs/component/component_registry.h"
#include "engine/utils/json/json_value.h"
#include "engine/rendering/font.h"
#include "engine/resources/engine_resources.h"
#include "engine/config/config_manager.h"

namespace cursed_engine
{
	namespace core_components
	{
		void registerAll(ComponentRegistry& registry, AssetManager& assetManager, EngineResources& engineResources, const ResourceConfig& resourceConfig)
		{
			// TODO; use config to check which subsystems are active, only register if active? (physics -> physicsComponent)

			registry.registerComponent<TransformComponent>("transform",
				[](EntityHandle& handle, const ComponentProperties& properties)
				{
					auto& transformComponent = handle.getComponent<TransformComponent>();
					//transformComponent.position = properties.at("position"); // WORKS???
				},
				[](EntityHandle& handle, const JsonValue& value)
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

			registry.registerComponent<SpriteComponent>("sprite",
				[](EntityHandle& handle, const ComponentProperties& properties)
				{
				},
				[&](EntityHandle& handle, const JsonValue& value)
				{
					std::string id = value["id"].asString();

					// [[consider]] if better to store only id at this point, and not reference any manager?
					const auto atlasHandle = assetManager.getAssetHandle<TextureAtlas>(id); // pass in id?

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

					std::array<float, 4> color{ 1.f, 1.f, 1.f, 1.f };

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
			registry.registerComponent<ButtonComponent>("button",
				[](EntityHandle& handle, const ComponentProperties& properties)
				{
				},
				[](EntityHandle& handle, const JsonValue& value)
				{
					std::string action = value["action"].asString();
					std::string val = value["value"].asString(); // asAny?

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

					handle.attachComponent<ButtonComponent>(action, val, defaultColor, optHoverColor, optPressedColor);
				});


			registry.registerComponent<BoundingBoxComponent>("bounding_box",
				[](EntityHandle& handle, const ComponentProperties& properties)
				{

				},
				[](EntityHandle& handle, const JsonValue& value)
				{
					int xOffset = value["offset"]["x"].asInt();
					int yOffset = value["offset"]["y"].asDouble();

					int width = value["size"]["width"].asInt() * 0.5f; // TODO; use halfextent instead!
					int height = value["size"]["height"].asInt() * 0.5f; // TODO; decide if float or int...

					handle.attachComponent<BoundingBoxComponent>(FVec2{ (float)xOffset, (float)yOffset }, FVec2{ (float)width, (float)height });
				});

			registry.registerComponent<TextComponent>("text",
				[](EntityHandle& handle, const ComponentProperties& properties)
				{

				},
				[&](EntityHandle& handle, const JsonValue& value)
				{
					//std::string id; // no way of knowing the id...

					auto id = value["text_id"].asString();

					std::string fontType = value["font"].asString();

					int fontSize = value["size"].asInt();
					// maybe just store id?

					//const auto& resourceIdToPath = resourceConfig.resourceIdToPath;

					auto fontHandle = engineResources.fontManager.getHandleById(fontType, fontSize);

					if (fontHandle.isValid())
					{
						handle.attachComponent<TextComponent>(id, fontHandle); // Only if suceesful??
					}
					else 
					{
						Logger::logWarning(std::format("[Constructing TextComponent from Json] - Failed to find path for font with id: {}", id));
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

			registry.registerComponent<AudioComponent>("audio",
				[](EntityHandle& handle, const ComponentProperties& properties)
				{},
				[&](EntityHandle& handle, const JsonValue& value)
				{
					std::string sound = value["sound"].asString();
					auto audioHandle = engineResources.audioManager.getHandleById(std::move(sound));

					if (!audioHandle.isValid())
					{

					}

					bool isLooping = value["should_loop"].asBool();

					// TODO; always check if contains? if no sound fail? if no is looping, default to false?

					handle.attachComponent<AudioComponent>(audioHandle, isLooping);
				});
		}
	}
}