#pragma once
#include "engine/resources/ref_counter.h"
#include <algorithm>
#include <cstdint>
#include <limits>

namespace cursed_engine
{
	// Does Handle free resource? 

	template <typename T>
	class ResHandle
	{
	public:
		explicit ResHandle(RefCounter<T>& refCounter);
		~ResHandle();

		// implement copy constructor / assignment...

		static constexpr ResHandle invalid();

		uint32_t getGeneration() const noexcept;
		uint32_t getIndex() const noexcept;

		bool operator==(const ResHandle& other) const noexcept;
		bool operator!=(const ResHandle& other) const noexcept;

	private:
		void release();
		void addRef();

		uint32_t m_generation;
		uint32_t m_index;
		
		RefCounter<T>& m_refCounter;
	};

#pragma region Methods

	template <typename T>
	ResHandle<T>::ResHandle(RefCounter<T>& refCounter)
		: m_refCounter{ refCounter }
	{
		addRef();
	}

	template <typename T>
	ResHandle<T>::~ResHandle()
	{
		release();
	}

	template <typename T>
	static constexpr ResHandle<T> ResHandle<T>::invalid()
	{
		return ResHandle(std::numeric_limits<uint32_t>::max(), 0);
	}

	template <typename T>
	uint32_t ResHandle<T>::getGeneration() const noexcept
	{
		return m_generation;
	}
	
	template <typename T>
	uint32_t ResHandle<T>::getIndex() const noexcept
	{
		return m_index;
	}

	template <typename T>
	void ResHandle<T>::release()
	{
		auto& refCount = m_refCounter.refCount;
		refCount = refCount ? refCount - 1 : 0;
	}

	template <typename T>
	void ResHandle<T>::addRef()
	{
		++m_refCounter.refCount;
	}

	template <typename T>
	bool ResHandle<T>::operator==(const ResHandle& other) const noexcept
	{
		return m_index == other.m_index && m_generation == other.m_generation;
	}

	template <typename T>
	bool ResHandle<T>::operator!=(const ResHandle& other) const noexcept
	{
		//return !(operator==(*this, other));
		return !(*this == other);
	}

#pragma endregion





	template <typename Tag>
	struct ResourceHandle
	{
		// TODO, add constexpr constructor

		// destructor => pointer to resource manager? decrement reference count?

		uint32_t index = std::numeric_limits<uint32_t>::max();
		uint32_t version = 0;

		// reference count? or store pointer to "reference block"?

		static constexpr ResourceHandle invalid()
		{
			return ResourceHandle(std::numeric_limits<uint32_t>::max(), 0);
		}

		//static const ResourceHandle invalid;

		bool isValid() const
		{
			return version >= 0 && index >= 0 && index < std::numeric_limits<uint32_t>::max(); // TODO; check!
		}

		bool operator==(const ResourceHandle& other) const
		{
			return index == other.index && version == other.version;
		}
	};

	//template <typename Tag>
	//const ResourceHandle<Tag> ResourceHandle<Tag>::invalid{ -1, 0 };

	//template <typename Tag> 
	//constexpr ResourceHandle<Tag> INVALID_RESOURCE_HANDLE{ -1, 0 }; // or static function?

	//struct TextureTag {};
	//struct AudioTag {};

	//using TextureHandle = ResourceHandle<TextureTag>;
	//using AudioHandle = ResourceHandle<AudioTag>;
}