#pragma once
#include "engine/core/subsystem.h"
#include "resource_handle.h" // Needed?
#include "resource_loader.h"
#include "resource_cache.hpp"

#include "engine/utils/path_utils.h"

#include <algorithm>
#include <filesystem> // put in pch
#include <memory>
#include <optional>

//#include "engine/rendering/texture.h" // DONT!!!!!!!

namespace cursed_engine
{
	// TODO; make sure to increment version!

	// Remaeke ResourceManager? resource base class? store 
	// TODO; add normalize path?
	// Document class does lazy loading!
	// Redo with, "id generator" that returns index to each type...


	//template <ResourceType Resource>
	//template <typename Resource>
	//struct ResourceArchive
	//{
	//	ResourceArchive(std::unique_ptr<ResourceLoaderBase<Resource>> loader_ = nullptr)
	//		: loader{ std::move(loader) }
	//	{
	//	}

	//	ResourceCache<Resource> cache;
	//	//std::unordered_map<std::string, std::string> pathsToIds; // use ids to paths instead!
	//	std::unordered_map<std::string, std::filesystem::path> idsToPaths;
	//	std::unordered_map<std::string, ResourceHandle<Resource>> idsToHandles;
	//	
	//	// id to meta data? ResourceMetaData...

	//	std::unique_ptr<ResourceLoaderBase<Resource>> loader; // templated overload instead of base classes
	//	mutable std::mutex mutex;
	//};

	//using fs = std::filesystem; --> put in types.h or something...



	// use Tag as well?
	template <typename T>
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		// ==================== Setup ====================

		template <typename Loader, typename... Args>
		void emplaceLoader(Args&&... args);

		void insertLoader(std::unique_ptr<ResourceLoaderBase<T>> loader);

		void insertPath(std::string id, std::filesystem::path path);

		// ==================== Resource acquisition ====================
		[[nodiscard]] ResourceHandle<T> getHandle(const std::string& id);

		[[nodiscard]] ResourceHandle<T> preload(const std::filesystem::path& path);

		// ==================== Resource access ====================
		[[nodiscard]] const T& get(ResourceHandle<T> handle) const;

		[[nodiscard]] T& get(ResourceHandle<T> handle);

		const T& operator[](ResourceHandle<T> handle) const;

		// ==================== Resource unloading / cleanup ====================
		void unload(const std::string& id);

		void unloadAll();

		void clear();

		// ==================== Queries ====================
		[[nodiscard]] bool isLoaded(const std::filesystem::path& path) const; // accept id instead+

	private:
		// ==================== Helpers ====================
		ResourceHandle<T> resolve(const std::filesystem::path& path);

		// ==================== Data Members ====================
		struct ResourceMetaData
		{
			ResourceHandle<T> handle;
			std::filesystem::path path;
		};

		std::unordered_map<std::string, ResourceMetaData> m_idToMetaData;
		std::unique_ptr<ResourceLoaderBase<T>> m_loader; // replace class with function ptr?
		ResourceCache<T> m_cache;
		mutable std::mutex m_mutex;
	};

#pragma region Methods

	template <typename T>
	template <typename Loader, typename... Args>
	void ResourceManager<T>::emplaceLoader(Args&&... args)
	{
		m_loader = std::make_unique<Loader>(std::forward<Args>(args)...);
	}

	template <typename T>
	void ResourceManager<T>::insertLoader(std::unique_ptr<ResourceLoaderBase<T>> loader)
	{
		m_loader = std::move(loader);
	}

	template <typename T>
	void ResourceManager<T>::insertPath(std::string id, std::filesystem::path path)
	{
		if (auto it = m_idToMetaData.find(id); it != m_idToMetaData.end())
		{
			it->second.path = std::move(path);
			it->second.handle = ResourceHandle<T>::invalid(); // TODO: make sure it's an invalid handle!
		}
		else
		{
			m_idToMetaData.insert({ std::move(id), ResourceMetaData{ ResourceHandle<T>::invalid(), std::move(path) }}); // TODO; make sure works!
		}
	}

	template <typename T>
	[[nodiscard]] ResourceHandle<T> ResourceManager<T>::getHandle(const std::string& id)
	{
		assert(m_idToMetaData.contains(id) && "Resource ID not found!");
		assert(std::filesystem::exists(m_idToMetaData.at(id).path) && "Invalid resource path");

		return resolve(m_idToMetaData[id].path);
	}

	template <typename T>
	[[nodiscard]] ResourceHandle<T> ResourceManager<T>::preload(const std::filesystem::path& path)
	{
		return resolve(path);
	}

	template <typename T>
	[[nodiscard]] const T& ResourceManager<T>::get(ResourceHandle<T> handle) const
	{
		// assert is valid handle?
		return m_cache.get(handle);
	}

	template <typename T>
	[[nodiscard]] T& ResourceManager<T>::get(ResourceHandle<T> handle)
	{
		return m_cache.get(handle);
	}

	template <typename T>
	const T& ResourceManager<T>::operator[](ResourceHandle<T> handle) const
	{
		assert(m_cache.isValidHandle(handle) && "ResourceManager::operator[] - not a valid handle");
		return m_cache.get(handle);
	}

	template <typename T>
	void ResourceManager<T>::unload(const std::string& id)
	{
		if (auto it = m_idToMetaData.find(id); it != m_idToMetaData.end())
		{
			auto& handle = it->second.handle;
			m_cache.remove(handle);

			handle = ResourceHandle<T>::invalid();
		}

		// TODO; else log error?
	}

	template <typename T>
	void ResourceManager<T>::unloadAll()
	{
		for (auto& [id, meta] : m_idToMetaData)
		{
			meta.handle = ResourceHandle<T>::invalid();
		}

		m_cache.clear();
	}

	template <typename T>
	void ResourceManager<T>::clear()
	{
		m_idToMetaData.clear();
		m_loader = nullptr;
		m_cache.clear();
	}

	template <typename T>
	[[nodiscard]] bool ResourceManager<T>::isLoaded(const std::filesystem::path& path) const
	{
		// chekc id? path? cache?

		return false;
	}

	template <typename T>
	ResourceHandle<T> ResourceManager<T>::resolve(const std::filesystem::path& path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		const std::string id = extractResourceID(path);

		// second lookup!? of meta data? - or fine?!
		if (auto it = m_idToMetaData.find(id); it != m_idToMetaData.end())
		{
			auto handle = it->second.handle;

			if (m_cache.isValidHandle(handle)) // why not just check handle itself? or both?
				return handle;
		}

		if (std::unique_ptr<T> resource = m_loader->load(path))
		{
			auto handle = m_cache.insert(std::move(resource));
			m_idToMetaData.insert_or_assign(id, ResourceMetaData{ handle, path });

			return handle;
		}

		return ResourceHandle<T>::invalid();
	}

#pragma endregion
} 