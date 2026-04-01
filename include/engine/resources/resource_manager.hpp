#pragma once
#include "engine/core/subsystem.h"
#include "engine/core/logger.h"
#include "resource_handle.h" // Needed?
#include "resource_loader.h"
#include "resource_cache.hpp"

#include "engine/utils/path_utils.h"

#include <algorithm>
#include <filesystem> // put in pch
#include <functional>
#include <memory>
#include <optional>

//#include "engine/rendering/texture.h" // DONT!!!!!!!

namespace cursed_engine
{
	template <typename T>
	struct ResourceKey // "overload"?
	{
	};

	// TODO; make sure to increment (handle) version! 
	// TODO; add normalize path?
	// TODO; add 'using fs = std::filesystem;' --> put in types.h or something...
	// Rename; ResourceStore? ResourceHandler?

	// Add documentation; class does lazy loading!
	// Redo with, "id generator" that returns index to each type...


	// Instead of "key" use "Params"?
	// use Tag as well? 
	// assert key contains path?
	template <typename Resource, typename Key, typename Hasher>
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		// ==================== Setup ====================

		template <typename Loader, typename... Args>
		void emplaceLoader(Args&&... args);

		void insertLoader(std::unique_ptr<ResourceLoader<Resource, Key>> loader);

		void insertPath(Key key, std::filesystem::path path);

		ResourceHandle<Resource> insertResource(Key key, std::unique_ptr<Resource> resource);

		// ==================== Resource acquisition ====================
		[[nodiscard]] ResourceHandle<Resource> getHandle(const Key& key); // Rename acquire?

		[[nodiscard]] ResourceHandle<Resource> preload(const Key& key);

		// ==================== Resource access ====================
		[[nodiscard]] const Resource& get(ResourceHandle<Resource> handle) const; // rename resolve?

		[[nodiscard]] Resource& get(ResourceHandle<Resource> handle);

		const Resource& operator[](ResourceHandle<Resource> handle) const; // add non const version?

		// ==================== Resource unloading / cleanup ====================
		void unload(const Key& key);

		void unloadAll();

		void clear();

		// ==================== Queries ====================
		[[nodiscard]] bool isLoaded(const Key& key) const;

		[[nodiscard]] bool isValidHandle(ResourceHandle<Resource> handle) const noexcept;

	private:
		// ==================== Helpers ====================
		ResourceHandle<Resource> resolve(const Key& key);

		// ==================== Data Members ====================
		struct ResourceMetaData
		{
			ResourceHandle<Resource> handle;
			std::filesystem::path path;
		};

		std::unordered_map<Key, ResourceMetaData, Hasher> m_keyToMetaData; // or map id to key with path?
		//std::unordered_map<std::string, ResourceMetaData> m_idToMetaData;
		std::unique_ptr<ResourceLoader<Resource, Key>> m_loader; // replace with function ptr?!
		//std::function<std::unique_ptr<Resource>(const std::filesystem::path& path, const Key& key)> m_loader;
		ResourceCache<Resource> m_cache;
		mutable std::mutex m_mutex; // TODO; use!
	};

#pragma region Methods

	template <typename Resource, typename Key, typename Hasher>
	template <typename Loader, typename... Args>
	void ResourceManager<Resource, Key, Hasher>::emplaceLoader(Args&&... args)
	{
		m_loader = std::make_unique<Loader>(std::forward<Args>(args)...);
	}

	template <typename Resource, typename Key, typename Hasher>
	void ResourceManager<Resource, Key, Hasher>::insertLoader(std::unique_ptr<ResourceLoader<Resource, Key>> loader)
	{
		m_loader = std::move(loader);
	}

	template <typename Resource, typename Key, typename Hasher>
	void ResourceManager<Resource, Key, Hasher>::insertPath(Key key, std::filesystem::path path)
	{
		if (auto it = m_keyToMetaData.find(key); it != m_keyToMetaData.end())
		{
			it->second.path = std::move(path);
			it->second.handle = ResourceHandle<Resource>::invalid(); // TODO: make sure it's an invalid handle!
		}
		else
		{
			m_keyToMetaData.insert({ std::move(key), ResourceMetaData{ ResourceHandle<Resource>::invalid(), std::move(path) }}); // TODO; make sure works!
		}
	}
	
	template <typename Resource, typename Key, typename Hasher>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Hasher>::insertResource(Key key, std::unique_ptr<Resource> resource)
	{
		if (m_keyToMetaData.contains(key))
		{
			Logger::logWarning("[ResourceManager::insertResource] - overwriting existing resource!");
			return ResourceHandle<Resource>::invalid();
		}

		auto handle = m_cache.insert(std::move(resource));
		m_keyToMetaData.insert({ std::move(key), ResourceMetaData{ handle, "" } });
	
		return handle;
	}
	
	template <typename Resource, typename Key, typename Hasher>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Hasher>::getHandle(const Key& key) // overload with vardiac template
	{
		assert(m_keyToMetaData.contains(key) && "Key not found!");

		// add key if not in map...

		return resolve(key); // or pass key?
		//return resolve(m_keyToMetaData[key]); // or pass key?
	}
	
	/*template <typename Resource, typename Key, typename Hasher>
	[[nodiscard]] ResourceHandle<Resource> ResourceManager<Resource, Key, Hasher>::getHandle(const std::string& id)
	{
		assert(m_idToMetaData.contains(id) && "Resource ID not found!");
		assert(std::filesystem::exists(m_idToMetaData.at(id).path) && "Invalid resource path");

		return resolve(m_idToMetaData[id].path);
	}*/

	template <typename Resource, typename Key, typename Hasher>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Hasher>::preload(const Key& key)
	{
		return resolve(key);
	}

	template <typename Resource, typename Key, typename Hasher>
	const Resource& ResourceManager<Resource, Key, Hasher>::get(ResourceHandle<Resource> handle) const
	{
		// assert is valid handle?
		return m_cache.get(handle);
	}

	template <typename Resource, typename Key, typename Hasher>
	Resource& ResourceManager<Resource, Key, Hasher>::get(ResourceHandle<Resource> handle)
	{
		return m_cache.get(handle);
	}

	template <typename Resource, typename Key, typename Hasher>
	const Resource& ResourceManager<Resource, Key, Hasher>::operator[](ResourceHandle<Resource> handle) const
	{
		assert(m_cache.isValidHandle(handle) && "ResourceManager::operator[] - not a valid handle");
		return m_cache.get(handle);
	}

	template <typename Resource, typename Key, typename Hasher>
	void ResourceManager<Resource, Key, Hasher>::unload(const Key& key)
	{
		if (auto it = m_keyToMetaData.find(key); it != m_keyToMetaData.end())
		{
			auto& handle = it->second.handle;
			m_cache.remove(handle);

			handle = ResourceHandle<Resource>::invalid();
		}

		// TODO; else log error?
	}

	template <typename Resource, typename Key, typename Hasher>
	void ResourceManager<Resource, Key, Hasher>::unloadAll()
	{
		for (auto& [key, meta] : m_keyToMetaData)
		{
			meta.handle = ResourceHandle<Resource>::invalid();
		}

		m_cache.clear();
	}

	template <typename Resource, typename Key, typename Hasher>
	void ResourceManager<Resource, Key, Hasher>::clear()
	{
		m_keyToMetaData.clear();
		m_loader = nullptr;
		m_cache.clear();
	}

	template <typename Resource, typename Key, typename Hasher>
	bool ResourceManager<Resource, Key, Hasher>::isLoaded(const Key& key) const
	{
		return m_keyToMetaData.contains(key);
	}

	template <typename Resource, typename Key, typename Hasher>
	bool ResourceManager<Resource, Key, Hasher>::isValidHandle(ResourceHandle<Resource> handle) const noexcept
	{
		return m_cache.isValidHandle(handle);
	}

	template <typename Resource, typename Key, typename Hasher>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Hasher>::resolve(const Key& key) // accept metadata as well?
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		//const std::string id = extractResourceID(key.path);

		// second lookup!? of meta data? - or fine?!
		if (auto it = m_keyToMetaData.find(key); it != m_keyToMetaData.end())
		{
			auto handle = it->second.handle;

			if (m_cache.isValidHandle(handle)) // why not just check handle itself? or both?
				return handle;
		
			const auto& path = it->second.path;
		
			if (std::unique_ptr<Resource> resource = m_loader->load(path, key))
			{
				auto handle = m_cache.insert(std::move(resource));
				m_keyToMetaData.insert_or_assign(key, ResourceMetaData{ handle, path });
				 
				return handle;
			}
		}

		return ResourceHandle<Resource>::invalid();
	}

	//template <typename Resource, typename Key, typename Hasher>
	//ResourceHandle<Resource> ResourceManager<Resource, Key, Hasher>::resolve(const Key& key)
	//{
	//	std::lock_guard<std::mutex> lock(m_mutex);

	//	const std::string id = extractResourceID(key.path);

	//	// second lookup!? of meta data? - or fine?!
	//	if (auto it = m_idToMetaData.find(id); it != m_idToMetaData.end())
	//	{
	//		auto handle = it->second.handle;

	//		if (m_cache.isValidHandle(handle)) // why not just check handle itself? or both?
	//			return handle;
	//	}

	//	if (std::unique_ptr<Resource> resource = m_loader->load(key.path))
	//	{
	//		auto handle = m_cache.insert(std::move(resource));
	//		m_idToMetaData.insert_or_assign(id, ResourceMetaData{ handle, path });

	//		return handle;
	//	}

	//	return ResourceHandle<Resource>::invalid();
	//}

#pragma endregion
}