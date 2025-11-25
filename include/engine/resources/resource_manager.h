#pragma once
#include "engine/core/subsystem.h"
#include "resource_handle.h" // Needed?

#include <filesystem> // put in pch
#include <memory>

namespace cursed_engine
{
	class Renderer;
	class Texture;
	class Audio;

	// TODO; use templated 'get' and 'resolve' functions?
	// TODO; add normalize path?
	// USE CASE; store handle in components
	 
	class ResourceManager : public Subsystem
	{
	public:
		ResourceManager(Renderer& renderer);
		~ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = default;

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = default;

		[[nodiscard]] TextureHandle getTexture(const std::filesystem::path& path);

		[[nodiscard]] Texture* resolve(TextureHandle handle);

		[[nodicard]] AudioHandle getAudio(const std::filesystem::path& path);

		[[nodiscard]] Audio* resolve(AudioHandle handle);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}