#pragma once
#include "resource_handle.h" // Needed?
#include <filesystem> // put in pch

namespace cursed_engine
{
	class Renderer;
	class Texture;
	// TODO; store handle in components

	// Rename: AssetHandler? AssetManager? ResourceStore?
	class ResourceManager
	{
	public:
		ResourceManager(Renderer& renderer);
		~ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = default; // Or delete??

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = default;

		[[nodiscard]] TextureHandle getTexture(const std::filesystem::path& path); // or just name? loader stores path?

		[[nodiscard]] Texture* resolve(TextureHandle handle);


	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;

		Renderer& m_renderer;
	};
}