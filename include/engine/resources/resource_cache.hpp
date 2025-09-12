#pragma once
#include "resource_handle.h"
#include <memory>
#include <mutex>
#include <cassert>
#include <filesystem>

namespace cursed_engine
{
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
