#pragma once
#include "engine/core/logger.h"
#include "engine/core/settings/engine_config.h"
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

	template <typename Resource, typename Descriptor, typename Loader>
	class ResourceManager final
	{
	public:
		ResourceManager();
		explicit ResourceManager(const ResourceConfig* resourceConfig, std::unique_ptr<Loader> loader);
		~ResourceManager() = default;

		void init(const ResourceConfig* resourceConfig, std::unique_ptr<Loader> loader);

		void update(uint64_t currentFrame, float deltaTime);

		// ==================== Resource insertion ====================
		ResourceHandle<Resource> insert(Descriptor descriptor, Resource resource);

		// ==================== Handle acquisition ====================
		[[nodiscard]] ResourceHandle<Resource> getHandle(const Descriptor& descriptor);

		template <typename... Args>
		[[nodiscard]] ResourceHandle<Resource> getHandleById(const std::string& id, Args&&... args); // rename getHandleByDescriptor? or dont accept args? just return null...
		
		[[nodiscard]] ResourceHandle<Resource> findHandle(const Descriptor& descriptor) const;

		// ==================== Resource access ====================
		[[nodiscard]] const Resource* get(ResourceHandle<Resource> handle) const;
		[[nodiscard]] Resource* get(ResourceHandle<Resource> handle);

		[[nodiscard]] const Resource& operator[](ResourceHandle<Resource> handle) const;
		[[nodiscard]] Resource& operator[](ResourceHandle<Resource> handle);

		// ==================== Resource unloading / cleanup ====================
		void unload(const Descriptor& descriptor); // release 
		
		void preload(const Descriptor& descriptor);

		void clear();

		// ==================== Queries ====================
		[[nodiscard]] bool contains(const Descriptor& descriptor) const noexcept;

		[[nodiscard]] bool isValid(ResourceHandle<Resource> handle) const noexcept;

	private:	
		// ==================== Type Alias ====================			
		using HandleMap = std::unordered_map<Descriptor, ResourceHandle<Resource>>;
	
		// ==================== Data Members ====================			
		ResourceCache<Resource> m_cache;
		const ResourceConfig* m_resourceConfig; // resource registry instead? (path, metadata, debug name)
		HandleMap m_descriptorToHandle;
		
		std::unique_ptr<Loader> m_loader;
	};

#pragma region Definitions

	template <typename Resource, typename Descriptor, typename Loader>
	ResourceManager<Resource, Descriptor, Loader>::ResourceManager()
		: m_cache{ 4000 }, m_loader{ nullptr } // TODO; FIX THIS!
	{
	}

	template <typename Resource, typename Descriptor, typename Loader>
	ResourceManager<Resource, Descriptor, Loader>::ResourceManager(const ResourceConfig* resourceConfig, std::unique_ptr<Loader> loader)
		: m_cache{ resourceConfig->framesBeforeUnload }, m_resourceConfig{ resourceConfig }, m_loader{ std::move(loader) }
	{
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::init(const ResourceConfig* resourceConfig, std::unique_ptr<Loader> loader)
	{
		m_resourceConfig = resourceConfig;
		m_loader = std::move(loader);
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::update(uint64_t currentFrame, float deltaTime)
	{
		m_cache.update(currentFrame);
	}
	
	template <typename Resource, typename Descriptor, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Descriptor, Loader>::insert(Descriptor descriptor, Resource resource)
	{
		auto handle = m_cache.store(std::move(resource));
		m_descriptorToHandle.insert_or_assign(std::move(descriptor), handle);
	
		return handle;
	}
	
	template <typename Resource, typename Descriptor, typename Loader>
	template <typename... Args>
	ResourceHandle<Resource> ResourceManager<Resource, Descriptor, Loader>::getHandleById(const std::string& id, Args&&... args)
	{
		// TODO; assert is not path...

		const auto& paths = m_resourceConfig->resourceIdToPath;

		if (auto it = paths.find(id); it != paths.end())
		{
			Descriptor descriptor{ it->second, std::forward<Args>(args)... }; // REQUIRES path to be first argument... use inheritance?

			return getHandle(descriptor); // map descriptors instead?
		}

		return ResourceHandle<Resource>::invalid();
	}

	template <typename Resource, typename Descriptor, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Descriptor, Loader>::getHandle(const Descriptor& descriptor)
	{
		if (auto it = m_descriptorToHandle.find(descriptor); it != m_descriptorToHandle.end())
		{
			ResourceHandle<Resource> handle = it->second;

			if (m_cache.isValid(handle))
				return handle;
		}

		Resource resource = (*m_loader)(descriptor); // TODO; fix... bit awkward..

		auto handle = m_cache.store(std::move(resource));
		m_descriptorToHandle.insert_or_assign(descriptor, handle);

		return handle;	
	}
	
	template <typename Resource, typename Descriptor, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Descriptor, Loader>::findHandle(const Descriptor& descriptor) const
	{
		if (auto it = m_descriptorToHandle.find(descriptor); it != m_descriptorToHandle.end())
		{
			ResourceHandle<Resource> handle = it->second;

			if (m_cache.isValid(handle))
				return handle;
		}

		return ResourceHandle<Resource>::invalid();
	}

	template <typename Resource, typename Descriptor, typename Loader>
	const Resource* ResourceManager<Resource, Descriptor, Loader>::get(ResourceHandle<Resource> handle) const
	{
		return m_cache.retrieve(handle);
	}

	template <typename Resource, typename Descriptor, typename Loader>
	Resource* ResourceManager<Resource, Descriptor, Loader>::get(ResourceHandle<Resource> handle)
	{
		return m_cache.retrieve(handle);
	}

	template <typename Resource, typename Descriptor, typename Loader>
	const Resource& ResourceManager<Resource, Descriptor, Loader>::operator[](ResourceHandle<Resource> handle) const
	{
		assert(m_cache.isValid(handle) && "ResourceManager::operator[] - not a valid handle");
		return m_cache[handle];
	}
	
	template <typename Resource, typename Descriptor, typename Loader>
	Resource& ResourceManager<Resource, Descriptor, Loader>::operator[](ResourceHandle<Resource> handle)
	{
		assert(m_cache.isValid(handle) && "ResourceManager::operator[] - not a valid handle");
		return m_cache[handle];
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::unload(const Descriptor& descriptor)
	{
		if (auto it = m_descriptorToHandle.find(descriptor); it != m_descriptorToHandle.end())
		{
			ResourceHandle<Resource>& handle = it->second;

			m_cache.release(handle);

			++handle.generation;
			handle.index = -1;
		}
		else
		{
			Logger::logWarning("[ResourceCache::unload] - Descriptor not found!");
		}
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::preload(const Descriptor& descriptor)
	{
		[[maybe_unusued]] auto handle = getHandle(descriptor); // FIX!? needed??
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::clear()
	{
		m_descriptorToHandle.clear();
		m_cache.clear();
	}

	template <typename Resource, typename Descriptor, typename Loader>
	bool ResourceManager<Resource, Descriptor, Loader>::contains(const Descriptor& descriptor) const noexcept
	{
		return m_descriptorToHandle.contains(descriptor);
	}

	template <typename Resource, typename Descriptor, typename Loader>
	bool ResourceManager<Resource, Descriptor, Loader>::isValid(ResourceHandle<Resource> handle) const noexcept
	{
		return m_cache.isValid(handle);
	}

#pragma endregion
}