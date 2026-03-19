#pragma once
#include "engine/core/subsystem.h"
#include "engine/resources/resource_manager.hpp"

// TODO; include resources instead? -> user of engine resources doesnt have to remember to include each resource...
// TODO; rename file? resource_fwd?
namespace cursed_engine
{
	class Texture;
	class Audio;
	class Font;

	//template <typename... Ts>
	//class ResourceManager;

	//using EngineResources = ResourceManager<Texture, Audio>;
	//using EngineResources = ResourceManager<Texture, Audio, Font>; // rename FileResources -> make struct?

	//struct EngineResources
	//{
	//	ResourceManager<Texture> textureManager;
	//	ResourceManager<Audio> audioManager;
	//};

	//template <typename T>
	//class ResourceManager;

	struct EngineResources : public Subsystem
	{
		ResourceManager<Texture> textureManager;
		ResourceManager<Audio> audioManager;
		ResourceManager<Font> fontManager;
	};
}