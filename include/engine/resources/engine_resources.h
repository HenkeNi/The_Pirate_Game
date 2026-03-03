#pragma once

// TODO; include resources instead? -> user of engine resources doesnt have to remember to include each resource...
// TODO; rename file? resource_fwd?
namespace cursed_engine
{
	class Texture;
	class Audio;
	class Font;

	template <typename... Ts>
	class ResourceManager;

	//using EngineResources = ResourceManager<Texture, Audio>;
	using EngineResources = ResourceManager<Texture, Audio, Font>;
}