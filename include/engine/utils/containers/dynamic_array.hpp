#pragma once
#include <memory>

namespace cursed_engine
{
	// initial size?
	// allocators?? allocator<T>?

	template <typename T, typename Alloc = std::allocator<T>>
	class dynamic_array
	{
	public:
		//explicit dynamic_array(const Alloc& alloc = Alloc());

		dynamic_array() = default;
		explicit dynamic_array(const Alloc& alloc);

		~dynamic_array();

		dynamic_array(const dynamic_array&) = delete;
		dynamic_array(dynamic_array&& other) noexcept;

		dynamic_array& operator=(const dynamic_array&) = delete;
		dynamic_array& operator=(dynamic_array&& other) noexcept;

		const T& operator[](std::size_t i) const;

		T& operator[](std::size_t i);

		template <typename... Args>
		void emplace_back(Args&&... args);
		
		void push_back(const T& value);

		void resize(std::size_t size);

		void reserve(std::size_t capacity);

		void clear();

		inline T* data() { return m_data; }

	private:
		Allocator m_alloc;
		T* m_data;
		std::size_t m_size = 0;
		std::size_t m_capacity = 0;

		static std::size_t s_initialCapacity = 8;
	};
	
#pragma region Definitions

	template <typename T, typename Alloc>
	explicit dynamic_array<T, Alloc>::dynamic_array(const Allocator& alloc = Allocator())
		: m_alloc{ alloc }, m_data{ alloc.allocate(s_initialCapacity) } 
	{
	}

	template <typename T, typename Alloc>
	dynamic_array<T, Alloc>::~dynamic_array()
	{
		clear();

		if (m_data)
		{
			alloc_traits::deallocate(m_alloc, m_data, m_capacity);
		}
	}

	template <typename T, typename Alloc>
	dynamic_array<T, Alloc>::dynamic_array(dynamic_array&& other) noexcept
		: m_data{ other.m_data }, m_size{ other.m_size }, m_capacity{ other.m_capacity }, m_alloc{ std::move(other.m_alloc) }
	{
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	template <typename T, typename Alloc>
	dynamic_array<T, Alloc>& dynamic_array<T, Alloc>::operator=(dynamic_array&& other) noexcept
	{
		if (this != &other)
		{
			clear();

			if (m_data)
			{
				alloc_traits::deallocate(m_alloc, m_data, m_capacity);
			}

			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_alloc = std::move(other.m_alloc);

			other.m_data = nullptr;
			other.m_size = 0;
			other.m_capacity = 0;
		}

		return *this;
	}

	template <typename T, typename Alloc>
	const T& dynamic_array<T, Alloc>::operator[](std::size_t i) const
	{

	}

	template <typename T, typename Alloc>
	T& dynamic_array<T, Alloc>::operator[](std::size_t i)
	{

	}

	template <typename T, typename Alloc>
	template <typename... Args>
	void dynamic_array<T, Alloc>::emplace_back(Args&&... args)
	{

	}

	template <typename T, typename Alloc>
	void dynamic_array<T, Alloc>::push_back(const T& value)
	{

	}

	template <typename T, typename Alloc>
	void dynamic_array<T, Alloc>::resize(std::size_t size)
	{

	}

	template <typename T, typename Alloc>
	void dynamic_array<T, Alloc>::reserve(std::size_t capacity)
	{

	}

	template <typename T, typename Alloc>
	void dynamic_array<T, Alloc>::clear()
	{

	}

#pragma endregion
}