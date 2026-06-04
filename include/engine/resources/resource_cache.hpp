#pragma once
#include "resource_handle.h"
#include <cassert>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace cursed_engine
{
	// [Consider] - maybe each resource should store how long before eviction?
	// [Consider] replacing stack (vector) with min heap?
	// [Consider] removing mutex (keep only in resource manager)
	// [Decide] ref count or last frame used? or both? or use: std::chrono::steady_clock::time_point lastUsed;
	// [TODO] - make thread safe? or not? Resource loading / handling needs to be done on main thread?

	template <typename T>
	class ResourceCache
	{
	private:		
		static_assert(std::is_default_constructible_v<T>, "ResourceCache requires T to be default constructible");

		using Handle = ResourceHandle<T>;

	public:
		ResourceCache(uint32_t framesBeforeEvict, std::size_t initialSize = 16);
		~ResourceCache() = default;

		// ==================== Lifecycle ====================
		void update(uint64_t frame);

		// ==================== Resource insertion ====================
		Handle store(T resource);

		// ==================== Resource access ====================
		[[nodiscard]] const T* retrieve(Handle handle) const;
		[[nodiscard]] T* retrieve(Handle handle);

		[[nodiscard]] const T& operator[](Handle handle) const;
		[[nodiscard]] T& operator[](Handle handle);

		// ==================== Resource eviction ====================
		void release(Handle handle);

		void clear();

		// ==================== Queries ====================
		[[nodiscard]] bool isValid(Handle handle) const noexcept;

	private:
		struct Entry
		{
			Entry(T resource, uint64_t frame)
				: resource{ std::move(resource) }, lastFrameUsed{ frame }
			{
			}

			T resource;
			uint32_t generation = 1;
			mutable uint64_t lastFrameUsed = 0;
			bool isAlive = true; 
		};

		// ==================== Helpers ====================
		[[nodiscard]] bool isValid(uint32_t index) const noexcept;

		void resetEntry(Entry& entry);

		void evictUnused();

		// ==================== Members ====================
		std::vector<Entry> m_entries;
		std::vector<std::size_t> m_freeList;

		uint64_t m_currentFrame = 0;
		const uint32_t m_elapsedFramesBeforeEviction;
	};

#pragma region Definitions

	template <typename T>
	ResourceCache<T>::ResourceCache(uint32_t framesBeforeEvict, std::size_t initialSize)
		: m_elapsedFramesBeforeEviction{ framesBeforeEvict }
	{
		m_entries.reserve(initialSize);
		m_freeList.reserve(initialSize);
	}

	template <typename T>
	void ResourceCache<T>::update(uint64_t currentFrame)
	{
		m_currentFrame = currentFrame;
		evictUnused();
	}

	template <typename T>
	ResourceCache<T>::Handle ResourceCache<T>::store(T resource)
	{
		if (m_freeList.empty())
		{
			m_entries.push_back(Entry{ std::move(resource), m_currentFrame });
			return ResourceCache<T>::Handle{ (uint32_t)m_entries.size() - 1, 1 };
		}

		std::size_t index = m_freeList.back();
		m_freeList.pop_back();

		auto& entry = m_entries[index];
		entry.resource = std::move(resource);
		entry.lastFrameUsed = m_currentFrame;
		entry.isAlive = true;

		return ResourceCache<T>::Handle{ (uint32_t)index, entry.generation };
	}

	template <typename T>
	const T* ResourceCache<T>::retrieve(Handle handle) const
	{
		return isValid(handle) ? &m_entries.at(handle.index).resource : nullptr;
	}

	template <typename T>
	T* ResourceCache<T>::retrieve(Handle handle)
	{
		return const_cast<T*>(std::as_const(*this).retrieve(handle));

	}

	template <typename T>
	const T& ResourceCache<T>::operator[](Handle handle) const
	{
		assert(isValid(handle) && "ResourceCache::operator[] - Invalid handle!");

		auto& entry = m_entries.at(handle.index);
		return entry.resource;
	}

	template <typename T>
	T& ResourceCache<T>::operator[](Handle handle)
	{
		assert(isValid(handle) && "ResourceCache::operator[] - Invalid handle!");

		auto& entry = m_entries.at(handle.index);
		return entry.resource;
	}

	template <typename T>
	void ResourceCache<T>::release(Handle handle)
	{
		if (!isValid(handle))
		{
			Logger::logWarning("[ResourceCaceh::evict] - Trying to evict with invalid handle");
			return;
		}

		uint32_t index = handle.index;
		resetEntry(m_entries[index]);

		m_freeList.push_back(index);
	}

	template <typename T>
	void ResourceCache<T>::clear()
	{
		m_entries.clear();
		m_freeList.clear();
	}

	template <typename T>
	bool ResourceCache<T>::isValid(Handle handle) const noexcept
	{
		return isValid(handle.index) && m_entries[handle.index].generation == handle.generation && m_entries[handle.index].isAlive;
	}

	template <typename T>
	bool ResourceCache<T>::isValid(uint32_t index) const noexcept
	{
		return index >= 0 && index < m_entries.size();
	}

	template<typename T>
	void ResourceCache<T>::resetEntry(Entry& entry)
	{
		entry.resource = T{}; // TODO; make sure all resources can be default initialized!
		entry.generation++;
		entry.isAlive = false;
	}

	template <typename T>
	void ResourceCache<T>::evictUnused()
	{
		for (int i = 0; i < m_entries.size(); ++i)
		{
			auto& entry = m_entries[i];
			// or just check if greater than?
			if (entry.isAlive && m_currentFrame - entry.lastFrameUsed >= m_elapsedFramesBeforeEviction)
			{
				resetEntry(entry);
				m_freeList.push_back(i);

				//Logger::logInfo("Offloaded resource");
			}
		}
	}

#pragma endregion
}