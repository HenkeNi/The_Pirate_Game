#pragma once
//#include "engine/core/settings/engine_config.h"
//#include "engine/core/file_catalog.h"
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

	// CONSIDER; Should ResourceManager stores the paths??????

	// use decltyp????????????????!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	template <typename T>
	class Result;

	template <typename Resource, typename Descriptor, typename Loader>
	class ResourceManager
	{
	public:
		// TODO; dont pass loader, construct it here!????????!?!?!?!?!?

		ResourceManager();
		explicit ResourceManager(uint32_t framesBeforeUnload, std::unique_ptr<Loader> loader);
		~ResourceManager() = default;

		void scanDirectory(const std::filesystem::path& root); // TOdo; find better way!

		// set frames before unload?
		void addLoader(std::unique_ptr<Loader>&& loader);

		void update(uint64_t currentFrame, float deltaTime);

		// ==================== Resource insertion ====================
		ResourceHandle<Resource> insert(Descriptor descriptor, std::unique_ptr<Resource> resource);

		// ==================== Handle acquisition ====================
		[[nodiscard]] ResourceHandle<Resource> getHandle(const Descriptor& descriptor);

		template <typename... Args>
		[[nodiscard]] ResourceHandle<Resource> getHandleById(const std::string& id, Args&&... args); // rename getHandleByDescriptor? or dont accept args? just return null...
		
		// TODO; rename tryFindHandle?
		[[nodiscard]] ResourceHandle<Resource> findHandle(const Descriptor& descriptor) const;

		// ==================== Resource access ====================
		[[nodiscard]] const Resource& get(ResourceHandle<Resource> handle) const;

		[[nodiscard]] const Resource* tryGet(ResourceHandle<Resource> handle) const;

		[[nodiscard]] const Resource& operator[](ResourceHandle<Resource> handle) const;

		// ==================== Resource unloading / cleanup ====================
		// ANY POINT TO THIS IF AUTOMATIC OFFLOADING:?!
		Result<void> unload(const Descriptor& descriptor); // release 
		
		// ANY POINT TO THIS IF AUTOMATIC OFFLOADING:?!
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
		//FileCatalog* m_fileCatalog;
		//const ResourceConfig* m_resourceConfig; // resource registry instead? (path, metadata, debug name)
		HandleMap m_descriptorToHandle;
		
		std::unique_ptr<Loader> m_loader; // does this need to be a pointer?

		std::unordered_map<std::string, std::string> m_idToPath;
		//std::unordered_map<std::string, std::filesystem::path> m_idToPath;

		// REsourcePool... (add a resource pool => pass to loader!)
	};

#pragma region Definitions

	template <typename Resource, typename Descriptor, typename Loader>
	ResourceManager<Resource, Descriptor, Loader>::ResourceManager()
		: m_cache{ 4000 }, m_loader{ nullptr } // TODO; FIX THIS!
	{
	}

	template <typename Resource, typename Descriptor, typename Loader>
	ResourceManager<Resource, Descriptor, Loader>::ResourceManager(uint32_t framesBeforeUnload, std::unique_ptr<Loader> loader)
		: m_cache{ framesBeforeUnload }, m_loader{ std::move(loader) }
	{
	}

	/*template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::init(PathMap resourceIdToPath, std::unique_ptr<Loader> loader)
	{
		m_resourceIdToPath = std::move(resourceIdToPath);
		m_loader = std::move(loader);
	}*/


	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::scanDirectory(const std::filesystem::path& root)
	{
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(root))
		{
			if (!entry.is_regular_file())
				continue;

			const std::filesystem::path& path = entry.path();
			const std::string filename = path.string();

			for (const std::string_view& extension : m_loader->extensions())
			{
				if (filename.ends_with(extension))
				{
					std::string filename = path.filename().string();

					std::string id = filename.substr(0, filename.length() - extension.length());

					m_idToPath.insert_or_assign(std::move(id), path.string());
					//m_idToPath.insert_or_assign(std::move(id), path);
				}
			}
		}
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::addLoader(std::unique_ptr<Loader>&& loader)
	{
		m_loader = std::move(loader);
	}

	template <typename Resource, typename Descriptor, typename Loader>
	void ResourceManager<Resource, Descriptor, Loader>::update(uint64_t currentFrame, float deltaTime)
	{
		m_cache.update(currentFrame);
	}
	
	template <typename Resource, typename Descriptor, typename Loader>
	ResourceHandle<Resource> ResourceManager<Resource, Descriptor, Loader>::insert(Descriptor descriptor, std::unique_ptr<Resource> resource)
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

		if (auto it = m_idToPath.find(id); it != m_idToPath.end())
		{
			Descriptor descriptor{ it->second, std::forward<Args>(args)... }; // REQUIRES path to be first argument... use inheritance?
			return getHandle(descriptor);
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
		
		//Resource resource = (*m_loader)(descriptor);
		Result<std::unique_ptr<Resource>> result = (*m_loader)(descriptor);

		if (result.ok())
		{
			auto handle = m_cache.store(std::move(result.take()));
			m_descriptorToHandle.insert_or_assign(descriptor, handle);

			return handle;	
		}

		return ResourceHandle<Resource>::invalid(); // Valid?
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
	const Resource& ResourceManager<Resource, Descriptor, Loader>::get(ResourceHandle<Resource> handle) const
	{
		return m_cache[handle];
	}

	template <typename Resource, typename Descriptor, typename Loader>
	const Resource* ResourceManager<Resource, Descriptor, Loader>::tryGet(ResourceHandle<Resource> handle) const
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
	Result<void> ResourceManager<Resource, Descriptor, Loader>::unload(const Descriptor& descriptor)
	{
		if (auto it = m_descriptorToHandle.find(descriptor); it != m_descriptorToHandle.end())
		{
			ResourceHandle<Resource>& handle = it->second;

			m_cache.release(handle);

			++handle.generation;
			handle.index = -1;

			return Result<void>::success();
		}
		
		return Result<void>::failure("Descriptor not found!");
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