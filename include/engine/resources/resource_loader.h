#pragma once
#include <memory>
#include <filesystem>
#include <future>

namespace cursed_engine
{
	class Audio;
	class Font;
	class Renderer;
	class Texture;

	struct TextureKey; // use include instead?
	struct AudioKey;
	struct FontKey;

	// Mutex?

	template <typename Resource, typename Key> // rename Key to ResourceMeta, Meta? Data? Info?
	class ResourceLoader
	{
	public:
		using ResourceType = Resource;

		virtual ~ResourceLoader() = default;


		// TODO; overload, one with path, other uses void*?  use std::any?
		[[nodiscard]] virtual std::unique_ptr<Resource> load(const std::filesystem::path& path, const Key& key) const = 0;
		//[[nodiscard]] virtual std::future<std::unique_ptr<Resource>> loadAsync(const std::filesystem::path& path) const = 0;
	};

	// Consider moving to resource_loaders.h?
	class TextureLoader : public ResourceLoader<Texture, TextureKey>
	{
	public:
		TextureLoader(Renderer& renderer);

		[[nodiscard]] std::unique_ptr<Texture> load(const std::filesystem::path& path, const TextureKey& key) const override;

		//[[nodiscard]] std::future<std::unique_ptr<Texture>> loadAsync(const std::filesystem::path& path) const override;

		//[[nodiscard]] bool isSupportedFormat(const std::string& format)

	private:

		Renderer& m_renderer;
		//static const std::vector<std::string> s_supportedExtensions;
	};


	class AudioLoader : public ResourceLoader<Audio, AudioKey>
	{
	public:
		[[nodiscard]] std::unique_ptr<Audio> load(const std::filesystem::path& path, const AudioKey& key) const override;
	};


	class FontLoader : public ResourceLoader<Font, FontKey>
	{
	public:
		[[nodiscard]] std::unique_ptr<Font> load(const std::filesystem::path& path, const FontKey& key) const override;
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