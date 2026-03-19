#pragma once
#include "resource_handle.h"
#include <memory>
#include <mutex>
#include <cassert>
#include <filesystem>
#include <numeric>

namespace cursed_engine
{
	// TODO; increase ref count?
	// [Consider] removing mutex (keep only in resource manager)

	class ResourceCacheBase
	{
	public:
		virtual ~ResourceCacheBase() = default;
	};

	// Should, or shouldn't accept Tag 
	template <typename Resource, std::size_t size = 1024>
	class ResourceCache
	{
	public:
		template <typename... Args>
		ResourceHandle<Resource> emplace(Args&&... args);

		ResourceHandle<Resource> insert(std::unique_ptr<Resource> resource);

		[[nodiscard]] const Resource& get(ResourceHandle<Resource> handle) const;

		[[nodiscard]] Resource& get(ResourceHandle<Resource> handle);

		[[nodiscard]] const Resource* tryGet(ResourceHandle<Resource> handle) const;

		[[nodiscard]] Resource* tryGet(ResourceHandle<Resource> handle);

		[[nodiscard]] bool isValidHandle(ResourceHandle<Resource> handle) const noexcept;
		
		void remove(ResourceHandle<Resource> handle);

		void clear(); // or reset?

	private:
		[[nodiscard]] bool isValidIndex(uint32_t index) const noexcept;

		struct Slot
		{
			std::unique_ptr<Resource> resource = nullptr;
			uint32_t version = 0;

			// automatic cleanup: bool?
			// store last frame used? or reference count? or refreence count in metadata?
		};

		std::vector<Slot> m_slots;
		std::vector<std::size_t> m_freeSlots;

		mutable std::mutex m_mutex;
	};

#pragma region Methods

	template <typename Resource, std::size_t size>
	template <typename... Args>
	ResourceHandle<Resource> ResourceCache<Resource, size>::emplace(Args&&... args)
	{
		// TODO; redundant function (remove)?
		std::lock_guard<std::mutex> lock(m_mutex);

		m_slots.emplace_back(std::make_unique<Args>(args)..., 0);
		return ResourceHandle<Resource>{ m_slots.size() - 1, 0 };
	}
	
	template <typename Resource, std::size_t size>
	ResourceHandle<Resource> ResourceCache<Resource, size>::insert(std::unique_ptr<Resource> resource)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_slots.push_back({ std::move(resource), 0 });
		return ResourceHandle<Resource>{ (uint32_t)m_slots.size() - 1, 0 };
	}

	template <typename Resource, std::size_t size>
	const Resource& ResourceCache<Resource, size>::get(ResourceHandle<Resource> handle) const
	{
		assert(isValidHandle(handle) && "ResourceCache::get - Invalid handle!"); // Redudnant? since manager is already checking!

		std::lock_guard<std::mutex> lock(m_mutex);

		auto& slot = m_slots.at(handle.index);
		return *slot.resource;
	}

	template <typename Resource, std::size_t size>
	Resource& ResourceCache<Resource, size>::get(ResourceHandle<Resource> handle)
	{
		return const_cast<Resource&>(std::as_const(*this).get(handle));
	}

	template <typename Resource, std::size_t size>
	const Resource* ResourceCache<Resource, size>::tryGet(ResourceHandle<Resource> handle) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		return isValidHandle(handle) ? m_slots.at(handle.index).resource.get() : nullptr;
	}

	template <typename Resource, std::size_t size>
	Resource* ResourceCache<Resource, size>::tryGet(ResourceHandle<Resource> handle)
	{
		return const_cast<Resource*>(std::as_const(*this).tryGet(handle));
	}

	template <typename Resource, std::size_t size>
	bool ResourceCache<Resource, size>::isValidHandle(ResourceHandle<Resource> handle) const noexcept
	{
		// mutex here (checking slots)
		return isValidIndex(handle.index) && (m_slots[handle.index].version == handle.version);

		return false;
	}

	template <typename Resource, std::size_t size>
	void ResourceCache<Resource, size>::remove(ResourceHandle<Resource> handle)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		
		assert(false && "Not implemented");
	}

	template <typename Resource, std::size_t size>
	void ResourceCache<Resource, size>::clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_slots.clear();
		m_freeSlots.clear();

		// Put in own function?
		m_freeSlots.resize(size);
		std::iota(m_freeSlots.begin(), m_freeSlots.end(), 0);

		// fill freeSlots with incremented number? (reveresed)... 
		// put free indixes into m_freeSlots...
	}

	template <typename Resource, std::size_t size>
	bool ResourceCache<Resource, size>::isValidIndex(uint32_t index) const noexcept
	{
		return index >= 0 && index < m_slots.size();
	}

#pragma endregion
}



/*
* #pragma once
#include "resource_handle.h"
#include <memory>
#include <mutex>
#include <cassert>
#include <filesystem>

namespace cursed_engine
{
	// TODO; increase ref count?
	// [Consider] removing mutex (keep only in resource manager)

	class ResourceCacheBase
	{
	public:
		virtual ~ResourceCacheBase() = default;
	};

	template <typename Res, typename Tag>
	class ResourceCache
	{
	public:
		template <typename... Args>
		ResourceHandle<Tag> emplace(Args&&... args);

		ResourceHandle<Tag> insert(std::unique_ptr<Res> resource);

		[[nodiscard]] const Res& get(ResourceHandle<Tag> handle) const;

		[[nodiscard]] Res& get(ResourceHandle<Tag> handle);

		[[nodiscard]] const Res* tryGet(ResourceHandle<Tag> handle) const;

		[[nodiscard]] Res* tryGet(ResourceHandle<Tag> handle);

		[[nodiscard]] bool isValidHandle(ResourceHandle<Tag> handle) const noexcept;
		
		void clear();

	private:
		[[nodiscard]] bool isValidIndex(uint32_t index) const noexcept;

		struct Slot
		{
			std::unique_ptr<Res> resource = nullptr;
			uint32_t version = 0;
		};

		std::vector<Slot> m_slots;
		mutable std::mutex m_mutex;
	};

#pragma region Methods

	template <typename Res, typename Tag>
	template <typename... Args>
	ResourceHandle<Tag> ResourceCache<Res, Tag>::emplace(Args&&... args)
	{
		// TODO; redundant function (remove)?
		std::lock_guard<std::mutex> lock(m_mutex);

		m_slots.emplace_back(std::make_unique<Args>(args)..., 0);
		return ResourceHandle<Tag>{ m_slots.size() - 1, 0 };
	}
	
	template <typename Res, typename Tag>
	ResourceHandle<Tag> ResourceCache<Res, Tag>::insert(std::unique_ptr<Res> resource)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_slots.push_back({ std::move(resource), 0 });
		return ResourceHandle<Tag>{ (uint32_t)m_slots.size() - 1, 0 };	
	}

	template <typename Res, typename Tag>
	const Res& ResourceCache<Res, Tag>::get(ResourceHandle<Tag> handle) const
	{
		assert(isValidHandle(handle) && "ResourceCache::get - Invalid handle!"); // Redudnant? since manager is already checking!

		std::lock_guard<std::mutex> lock(m_mutex);

		auto& slot = m_slots.at(handle.index);
		return *slot.resource;
	}

	template <typename Res, typename Tag>
	Res& ResourceCache<Res, Tag>::get(ResourceHandle<Tag> handle)
	{
		return const_cast<Res&>(std::as_const(*this).get(handle));
	}

	template <typename Res, typename Tag>
	const Res* ResourceCache<Res, Tag>::tryGet(ResourceHandle<Tag> handle) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		return isValidHandle(handle) ? m_slots.at(handle.index).resource.get() : nullptr;
	}

	template <typename Res, typename Tag>
	Res* ResourceCache<Res, Tag>::tryGet(ResourceHandle<Tag> handle)
	{
		return const_cast<Res*>(std::as_const(*this).tryGet(handle));
	}

	template <typename Res, typename Tag>
	bool ResourceCache<Res, Tag>::isValidHandle(ResourceHandle<Tag> handle) const noexcept
	{
		// mutex here (checking slots)
		return isValidIndex(handle.index) && (m_slots[handle.index].version == handle.version);

		return false;
	}

	template <typename Res, typename Tag>
	void ResourceCache<Res, Tag>::clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_slots.clear();
	}

	template <typename Res, typename Tag>
	bool ResourceCache<Res, Tag>::isValidIndex(uint32_t index) const noexcept
	{
		return index >= 0 && index < m_slots.size();
	}

#pragma endregion
}

*/