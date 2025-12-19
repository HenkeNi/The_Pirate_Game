#pragma once
#include <memory>
#include <filesystem>

namespace cursed_engine
{
	class Audio;
	class Renderer;
	class Texture;

	template <typename Resource>
	class ResourceLoaderBase // rename ResourceLoader?
	{
	public:
		using ResourceType = Resource;

		virtual ~ResourceLoaderBase() = default;
		
		[[nodiscard]] virtual std::unique_ptr<Resource> load(const std::filesystem::path& path) const = 0;
	};


	class TextureLoader : public ResourceLoaderBase<Texture>
	{
	public:
		TextureLoader(Renderer& renderer);

		[[nodiscard]] std::unique_ptr<Texture> load(const std::filesystem::path& path) const override;
	
	private:
		Renderer& m_renderer;
	};


	class AudioLoader : public ResourceLoaderBase<Audio>
	{
	public:
		[[nodiscard]] std::unique_ptr<Audio> load(const std::filesystem::path& path) const override;
	};


//	class ResourceLoader
//	{
//	public:
//
//		template <typename Resource>
//		[[nodiscard]] std::unique_ptr<Resource> load(const std::filesystem::path& path) const;
//	}; 
//
//#pragma region Methods
//
//	template <typename Resource, typename... Args>
//	[[nodiscard]] std::unique_ptr<Resource> ResourceLoader::load(const std::filesystem::path& path, Args&&... args) const
//	{
//		assert(false && "Resource Type not handled!");
//	}
//
//	template <>
//	[[nodiscard]] std::unique_ptr<Texture> load(const std::filesystem::path& path, Renderer& renderer) const
//	{
//
//	}
//
//	template <>
//	[[nodiscard]] std::unique_ptr<Audio> load(const std::filesystem::path& path) const
//	{
//
//	}
//
//#pragma endregion
}