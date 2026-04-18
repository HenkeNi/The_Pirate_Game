#pragma once
#include "engine/core/logger.h"
#include "engine/config/config_manager.h"
#include "resource_handle.h"
#include "resource_cache.hpp"
#include <algorithm>
#include <filesystem>
#include <functional>
#include <memory>

namespace cursed_engine
{
	// TODO; add normalize path? use Tag as well? 
	// Add documentation; class does lazy loading! unsafe to store resources...
	// Handle if resource doesnt unload...
	// Find better name; ResourceService, ResourceHandler? ResourceStore? ResourceAcessor? ResourceCache

	template <typename Resource, typename Key, typename Loader>
	class ResourceManager final
	{
	public:
		explicit ResourceManager(const ResourceConfig& resourceConfig, Loader loader);
		~ResourceManager() = default;

		void update(uint64_t currentFrame, float deltaTime);

		// ==================== Resource insertion ====================
		ResourceHandle<Resource> insert(Key key, Resource resource);

		// ==================== Handle acquisition ====================
		[[nodiscard]] ResourceHandle<Resource> getHandle(const Key& key);

		template <typename... Args>
		[[nodiscard]] ResourceHandle<Resource> getHandleById(const std::string& id, Args&&... args);
		
		[[nodiscard]] ResourceHandle<Resource> findHandle(const Key& key) const;

		// ==================== Resource access ====================
		[[nodiscard]] const Resource* get(ResourceHandle<Resource> handle) const;
		[[nodiscard]] Resource* get(ResourceHandle<Resource> handle);

		[[nodiscard]] const Resource& operator[](ResourceHandle<Resource> handle) const;
		[[nodiscard]] Resource& operator[](ResourceHandle<Resource> handle);

		// ==================== Resource unloading / cleanup ====================
		void unload(const Key& key); // release 
		
		void preload(const Key& key);

		void clear();

		// ==================== Queries ====================
		[[nodiscard]] bool contains(const Key& key) const noexcept;

		[[nodiscard]] bool isValid(ResourceHandle<Resource> handle) const noexcept;

	private:	
		// ==================== Type Alias ====================			
		using HandleMap = std::unordered_map<Key, ResourceHandle<Resource>>;
	
		// ==================== Data Members ====================			
		ResourceCache<Resource> m_cache;
		const ResourceConfig& m_resourceConfig; // resource registry instead? (path, metadata, debug name)
		HandleMap m_keyToHandle;
		Loader m_loader;
	};

#pragma region Methods

	template <typename Resource, typename Key, typename Loader>
	ResourceManager<Resource, Key, Loader>::ResourceManager(const ResourceConfig& resourceConfig, Loader loader)
		: m_cache{ resourceConfig.framesBeforeUnload }, m_resourceConfig{ resourceConfig }, m_loader{ std::move(loader) }
	{
	}

	template <typename Resource, typename Key, typename Loader>
	void ResourceManager<Resource, Key, Loader>::update(uint64_t currentFrame, float deltaTime)
	{
		m_cache.update(currentFrame);
	}
	
	template <typename Resource, typename Key, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Loader>::insert(Key key, Resource resource)
	{
		auto handle = m_cache.store(std::move(resource));
		m_keyToHandle.insert_or_assign(std::move(key), handle);
	
		return handle;
	}
	
	template <typename Resource, typename Key, typename Loader>
	template <typename... Args>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Loader>::getHandleById(const std::string& id, Args&&... args)
	{
		const auto& paths = m_resourceConfig.resourceIdToPath;

		if (auto it = paths.find(id); it != paths.end())
		{
			Key key{ it->second, std::forward<Args>()... };

			return getHandle(key);
		}

		return ResourceHandle<Resource>::invalid();
	}

	template <typename Resource, typename Key, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Loader>::getHandle(const Key& key)
	{
		if (auto it = m_keyToHandle.find(key); it != m_keyToHandle.end())
		{
			ResourceHandle<Resource> handle = it->second;

			if (m_cache.isValid(handle))
				return handle;
		}

		Resource resource = m_loader(key);		

		auto handle = m_cache.store(std::move(resource));
		m_keyToHandle.insert_or_assign(key, handle);

		return handle;	
	}
	
	template <typename Resource, typename Key, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Key, Loader>::findHandle(const Key& key) const
	{
		if (auto it = m_keyToHandle.find(key); it != m_keyToHandle.end())
		{
			ResourceHandle<Resource> handle = it->second;

			if (m_cache.isValid(handle))
				return handle;
		}

		return ResourceHandle<Resource>::invalid();
	}

	template <typename Resource, typename Key, typename Loader>
	const Resource* ResourceManager<Resource, Key, Loader>::get(ResourceHandle<Resource> handle) const
	{
		return m_cache.retrieve(handle);
	}

	template <typename Resource, typename Key, typename Loader>
	Resource* ResourceManager<Resource, Key, Loader>::get(ResourceHandle<Resource> handle)
	{
		return m_cache.retrieve(handle);
	}

	template <typename Resource, typename Key, typename Loader>
	const Resource& ResourceManager<Resource, Key, Loader>::operator[](ResourceHandle<Resource> handle) const
	{
		assert(m_cache.isValid(handle) && "ResourceManager::operator[] - not a valid handle");
		return m_cache[handle];
	}
	
	template <typename Resource, typename Key, typename Loader>
	Resource& ResourceManager<Resource, Key, Loader>::operator[](ResourceHandle<Resource> handle)
	{
		assert(m_cache.isValid(handle) && "ResourceManager::operator[] - not a valid handle");
		return m_cache[handle];
	}

	template <typename Resource, typename Key, typename Loader>
	void ResourceManager<Resource, Key, Loader>::unload(const Key& key)
	{
		if (auto it = m_keyToHandle.find(key); it != m_keyToHandle.end())
		{
			ResourceHandle<Resource>& handle = it->second;

			m_cache.release(handle);

			++handle.generation;
			handle.index = -1;
		}
		else
		{
			Logger::logWarning("[ResourceCache::unload] - Key not found!");
		}
	}

	template <typename Resource, typename Key, typename Loader>
	void ResourceManager<Resource, Key, Loader>::preload(const Key& key)
	{
		[[maybe_unusued]] auto handle = getHandle(key); // FIX!? needed??
	}

	template <typename Resource, typename Key, typename Loader>
	void ResourceManager<Resource, Key, Loader>::clear()
	{
		m_keyToHandle.clear();
		m_cache.clear();
	}

	template <typename Resource, typename Key, typename Loader>
	bool ResourceManager<Resource, Key, Loader>::contains(const Key& key) const noexcept
	{
		return m_keyToHandle.contains(key);
	}

	template <typename Resource, typename Key, typename Loader>
	bool ResourceManager<Resource, Key, Loader>::isValid(ResourceHandle<Resource> handle) const noexcept
	{
		return m_cache.isValid(handle);
	}

#pragma endregion
}