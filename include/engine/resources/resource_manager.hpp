#pragma once
#include "engine/core/subsystem.h"
#include "resource_handle.h" // Needed?
#include "resource_loader.h"
#include "resource_cache.hpp"

#include <filesystem> // put in pch
#include <memory>
#include <optional>

#include "engine/rendering/texture.h" // DONT!!!!!!!

namespace cursed_engine
{
	// Remaeke ResourceManager? resource base class? store 
	// TODO; add normalize path?
	// Document class does lazy loading!
	// Redo with, "id generator" that returns index to each type...


	//template <ResourceType Resource>
	template <typename Resource>
	struct ResourceArchive
	{
		ResourceArchive(std::unique_ptr<ResourceLoaderBase<Resource>> loader_ = nullptr)
			: loader{ std::move(loader) }
		{
		}

		ResourceCache<Resource> cache;
		//std::unordered_map<std::string, std::string> pathsToIds; // use ids to paths instead!
		std::unordered_map<std::string, std::filesystem::path> idsToPaths;
		std::unordered_map<std::string, ResourceHandle<Resource>> idsToHandles;
		
		std::unique_ptr<ResourceLoaderBase<Resource>> loader; // templated overload instead of base classes
		mutable std::mutex mutex;
	};


	//class Texture;
	//class Audio;
	
	
	//template <typename... Ts>
	//class ResourceManager;

	//using EngineResources = ResourceManager<Texture, Audio>; // put in own header file?

	template <typename... Ts>
	class ResourceManager final : public Subsystem
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		// ==================== Setup ====================
		template <typename Loader, typename... Args>
		void registerLoader(Args&&... args);

		template <typename Resource>
		void insertLoader(std::unique_ptr<ResourceLoaderBase<Resource>> loader);

		template <typename Resource>
		void insertPath(const std::string& id, const std::filesystem::path& path);

		// ==================== Resource acquisition ====================
		template <typename Resource>
		[[nodiscard]] ResourceHandle<Resource> getHandle(const std::string& id);

		template <typename Resource>
		[[nodiscard]] ResourceHandle<Resource> preload(const std::filesystem::path& path);

		// ==================== Resource access ====================
		template <typename Resource>
		[[nodiscard]] const Resource& get(ResourceHandle<Resource> handle) const;

		template <typename Resource>
		[[nodiscard]] Resource& get(ResourceHandle<Resource> handle);

		template <typename Resource>
		const Resource& operator[](ResourceHandle<Resource> handle) const;

		// ==================== Resource unloading / cleanup ====================
		template <typename Resource>
		void unload(ResourceHandle<Resource> handle);

		template <typename Resource>
		void unloadAll();

		void clear();
		
		// ==================== Queries ====================
		template <typename Resource>
		[[nodiscard]] bool isLoaded(const std::filesystem::path& path) const;

	private:
		// ==================== Helpers ====================
		template <typename Resource>
		const ResourceArchive<Resource>& getResourceArchive() const;
		
		template <typename Resource>
		ResourceArchive<Resource>& getResourceArchive();

		template <typename Resource>
		ResourceHandle<Resource> resolve(const std::filesystem::path& path);

		std::tuple<ResourceArchive<Ts>...> m_archives;
	};

#pragma region Methods

	template <typename... Ts>
	template <typename Loader, typename... Args>
	void ResourceManager<Ts...>::registerLoader(Args&&... args)
	{
		auto& archive = getResourceArchive<typename Loader::ResourceType>();  // DONT WORK?!
		archive.loader = std::make_unique<Loader>(std::forward<Args>(args)...);
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::insertLoader(std::unique_ptr<ResourceLoaderBase<Resource>> loader)
	{
		auto& entry = getResourceArchive<Resource>();
		entry.loader = std::move(loader);
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::insertPath(const std::string& id, const std::filesystem::path& path)
	{
		auto& archive = getResourceArchive<Resource>();
		archive.idsToPaths.insert({ id, path });
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] ResourceHandle<Resource> ResourceManager<Ts...>::getHandle(const std::string& id)
	{
		auto& archive = getResourceArchive<Resource>();

		// TODO; assert if filepath and not id...
		assert(archive.idsToPaths.contains(id) && "ID not found among resource paths!");

		return resolve<Resource>(archive.idsToPaths[id]);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] ResourceHandle<Resource> ResourceManager<Ts...>::preload(const std::filesystem::path& path)
	{
		return resolve<Resource>(path);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] const Resource& ResourceManager<Ts...>::get(ResourceHandle<Resource> handle) const
	{
		const auto& archive = getResourceArchive<Resource>();
		return archive.cache.get(handle);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] Resource& ResourceManager<Ts...>::get(ResourceHandle<Resource> handle)
	{
		auto& archive = getResourceArchive<Resource>();
		return archive.cache.get(handle);
	}

	template <typename... Ts>
	template <typename Resource>
	const Resource& ResourceManager<Ts...>::operator[](ResourceHandle<Resource> handle) const
	{
		auto& archive = getResourceArchive<Resource>();
		assert(archive.cache.isValidHandle(handle) && "ResourceManager::operator[] - not a valid handle");

		return archive.cache.get(handle);
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::unload(ResourceHandle<Resource> handle)
	{
		auto& archive = getResourceArchive<Resource>();
		
		// TODO; how?
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::unloadAll()
	{
		auto& archive = getResourceArchive<Resource>();
		archive.cache.clear();
		archive.pathsToIds.clear();
		archive.idsToHandles.clear();
	}

	template <typename... Ts>
	void ResourceManager<Ts...>::clear()
	{

	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] bool ResourceManager<Ts...>::isLoaded(const std::filesystem::path& path) const
	{
		auto& archive = getResourceArchive<Resource>();
		
		// TODO; maybe check if cache contains id as well? maybe path is not enough?

		return archive.pathToIds.contains(path);
	}

	template <typename... Ts>
	template <typename Resource>
	const ResourceArchive<Resource>& ResourceManager<Ts...>::getResourceArchive() const
	{
		return std::get<ResourceArchive<Resource>>(m_archives);
	}

	template <typename... Ts>
	template <typename Resource>
	ResourceArchive<Resource>& ResourceManager<Ts...>::getResourceArchive()
	{
		return std::get<ResourceArchive<Resource>>(m_archives);
	}

	template <typename... Ts>
	template <typename Resource>
	ResourceHandle<Resource> ResourceManager<Ts...>::resolve(const std::filesystem::path& path)
	{
		auto& archive = getResourceArchive<Resource>();

		std::lock_guard<std::mutex> lock(archive.mutex);

		const std::string identifier = path.filename().string();
		const std::string keyPath = path.string();

		if (auto it = archive.idsToHandles.find(identifier); it != archive.idsToHandles.end())
		{
			auto handle = it->second;

			if (archive.cache.isValidHandle(handle))
				return handle;
			else
			{
				//archive.pathsToIds.erase(keyPath); erase ids to paths to?
				archive.idsToHandles.erase(identifier);
			}
		}

		std::unique_ptr<Resource> resource = archive.loader->load(path);
		auto handle = archive.cache.insert(std::move(resource));

		archive.idsToHandles.insert({ identifier, handle });
		//archive.pathsToIds.insert({ keyPath, identifier });

		return handle;
	}

#pragma endregion
} 



/*
#pragma once
#include "engine/core/subsystem.h"
#include "resource_handle.h" // Needed?
#include "resource_loader.h"
#include "resource_cache.hpp"

#include <filesystem> // put in pch
#include <memory>
#include <optional>

namespace cursed_engine
{
	// Remaeke ResourceManager? resource base class? store
	// TODO; add normalize path?
	// Document class does lazy loading!
	// Redo with, "id generator" that returns index to each type...

	class Audio
	{
	public:
		struct Tag{};
	};

	//template <typename T>
	//concept ResourceType = requires {
	//	typename T::Tag;
	//};

	//template <ResourceType Resource>
	template <typename Resource>
	struct ResourceArchive
	{
		using Tag = typename Resource::Tag; // need to see resource::Tag...

		ResourceArchive(std::unique_ptr<ResourceLoaderBase<Resource>> loader_ = nullptr)
			: loader{ std::move(loader) }
		{
		}

		ResourceCache<Resource, Tag> cache;
		//std::unordered_map<std::string, std::string> pathsToIds; // use ids to paths instead!
		std::unordered_map<std::string, std::filesystem::path> idsToPaths;
		std::unordered_map<std::string, ResourceHandle<Tag>> idsToHandles;

		std::unique_ptr<ResourceLoaderBase<Resource>> loader; // templated overload instead of base classes
		mutable std::mutex mutex;
	};


	//class Texture;
	//class Audio;
	class Engine;

	//template <typename... Ts>
	//class ResourceManager;

	//using EngineResources = ResourceManager<Texture, Audio>; // put in own header file?

	template <typename... Ts>
	class ResourceManager final : public Subsystem
	{
	public:
		~ResourceManager() = default;

		// ==================== Setup ====================
		template <typename Loader, typename... Args>
		void registerLoader(Args&&... args);

		template <typename Resource>
		void insertLoader(std::unique_ptr<ResourceLoaderBase<Resource>> loader);

		template <typename Resource>
		void insertPath(const std::filesystem::path& path, const std::string& id);

		// ==================== Resource acquisition ====================
		template <typename Resource>
		[[nodiscard]] ResourceHandle<typename Resource::Tag> getHandle(const std::string& id);

		template <typename Resource>
		[[nodiscard]] ResourceHandle<typename Resource::Tag> preload(const std::filesystem::path& path);

		// ==================== Resource access ====================
		template <typename Resource>
		[[nodiscard]] const Resource& get(ResourceHandle<typename Resource::Tag> handle) const;

		template <typename Resource>
		[[nodiscard]] Resource& get(ResourceHandle<typename Resource::Tag> handle);

		template <typename Resource>
		const Resource& operator[](ResourceHandle<typename Resource::Tag> handle) const;

		// ==================== Resource unloading / cleanup ====================
		template <typename Resource>
		void unload(ResourceHandle<typename Resource::Tag> handle);

		template <typename Resource>
		void unloadAll();

		void clear();

		// ==================== Queries ====================
		template <typename Resource>
		[[nodiscard]] bool isLoaded(const std::filesystem::path& path) const;

	private:
		friend class Engine;

		ResourceManager() = default;

		// ==================== Helpers ====================
		template <typename Resource>
		const ResourceArchive<Resource>& getResourceArchive() const;

		template <typename Resource>
		ResourceArchive<Resource>& getResourceArchive();

		template <typename Resource>
		ResourceHandle<typename Resource::Tag> resolve(const std::filesystem::path& path);

		std::tuple<ResourceArchive<Ts>...> m_archives;
	};

#pragma region Methods

	template <typename... Ts>
	template <typename Loader, typename... Args>
	void ResourceManager<Ts...>::registerLoader(Args&&... args)
	{
		auto& entry = getResourceArchive<Loader::ResourceType>();
		entry.loader = std::make_unique<Loader>(std::forward<Args>(args...));
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::insertLoader(std::unique_ptr<ResourceLoaderBase<Resource>> loader)
	{
		auto& entry = getResourceArchive<Resource>();
		entry.loader = std::move(loader);
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::insertPath(const std::filesystem::path& path, const std::string& id)
	{
		auto& archive = getResourceArchive<Resource>();
		archive.idsToPaths.insert(id, path);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] ResourceHandle<typename Resource::Tag> ResourceManager<Ts...>::getHandle(const std::string& id)
	{
		auto& archive = getResourceArchive<Resource>();
		assert(archive.idsToPaths.contains(id) && "ID not found among resource paths!");

		return resolve<Resource>(archive.idsToPaths[id]);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] ResourceHandle<typename Resource::Tag> ResourceManager<Ts...>::preload(const std::filesystem::path& path)
	{
		return resolve<Resource>(path);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] const Resource& ResourceManager<Ts...>::get(ResourceHandle<typename Resource::Tag> handle) const
	{
		const auto& archive = getResourceArchive<Resource>();
		return archive.cache.get(handle);
	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] Resource& ResourceManager<Ts...>::get(ResourceHandle<typename Resource::Tag> handle)
	{
		auto& archive = getResourceArchive<Resource>();
		return archive.cache.get(handle);
	}

	template <typename... Ts>
	template <typename Resource>
	const Resource& ResourceManager<Ts...>::operator[](ResourceHandle<typename Resource::Tag> handle) const
	{
		auto& archive = getResourceArchive<Resource>();
		assert(archive.cache.isValidHandle(handle) && "ResourceManager::operator[] - not a valid handle");

		return archive.cache.get(handle);
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::unload(ResourceHandle<typename Resource::Tag> handle)
	{
		auto& archive = getResourceArchive<Resource>();

		// TODO; how?
	}

	template <typename... Ts>
	template <typename Resource>
	void ResourceManager<Ts...>::unloadAll()
	{
		auto& archive = getResourceArchive<Resource>();
		archive.cache.clear();
		archive.pathsToIds.clear();
		archive.idsToHandles.clear();
	}

	template <typename... Ts>
	void ResourceManager<Ts...>::clear()
	{

	}

	template <typename... Ts>
	template <typename Resource>
	[[nodiscard]] bool ResourceManager<Ts...>::isLoaded(const std::filesystem::path& path) const
	{
		auto& archive = getResourceArchive<Resource>();

		// TODO; maybe check if cache contains id as well? maybe path is not enough?

		return archive.pathToIds.contains(path);
	}

	template <typename... Ts>
	template <typename Resource>
	const ResourceArchive<Resource>& ResourceManager<Ts...>::getResourceArchive() const
	{
		return std::get<ResourceArchive<Resource>>(m_archives);
	}

	template <typename... Ts>
	template <typename Resource>
	ResourceArchive<Resource>& ResourceManager<Ts...>::getResourceArchive()
	{
		return std::get<ResourceArchive<Resource>>(m_archives);
	}

	template <typename... Ts>
	template <typename Resource>
	ResourceHandle<typename Resource::Tag> ResourceManager<Ts...>::resolve(const std::filesystem::path& path)
	{
		auto& archive = getResourceArchive<Resource>();

		std::lock_guard<std::mutex> lock(archive.mutex);

		const std::string identifier = path.filename().string();
		const std::string keyPath = path.string();

		if (auto it = archive.idsToHandles.find(identifier); it != archive.idsToHandles.end())
		{
			auto handle = it->second;

			if (archive.cache.isHandleValid(handle))
				return handle;
			else
			{
				//archive.pathsToIds.erase(keyPath); erase ids to paths to?
				archive.idsToHandles.erase(identifier);
			}
		}

		std::unique_ptr<Resource> resource = archive.loader.load(path);
		auto handle = archive.cache.insert(std::move(resource));

		archive.idsToHandles.insert({ identifier, handle });
		//archive.pathsToIds.insert({ keyPath, identifier });

		return handle;
	}

#pragma endregion
}
*/