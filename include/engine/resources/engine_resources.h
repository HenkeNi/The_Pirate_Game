#pragma once

namespace cursed_engine
{
	class Texture;
	class Audio;

	template <typename... Ts>
	class ResourceManager;

	using EngineResources = ResourceManager<Texture, Audio>;
}