#pragma once

namespace cursed_engine
{
	template<typename Container>
	class GenericIterator
	{
	public:
		using T = Container;
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		// TODO; mark constexpr or not?

		// ==================== Constructors ====================
		//base_iterator() noexcept : m_ptr{ nullptr } {} // remove?
		GenericIterator(pointer ptr) noexcept : m_ptr{ ptr } {} // why ptr and not ref?

		// ==================== Dereference ====================
		reference operator*() const noexcept { return *m_ptr; }
		pointer operator->() const noexcept { return m_ptr; }
		reference operator[](difference_type n) const noexcept { return *(m_ptr + n); }

		// ==================== Increment / Decrement ====================
		GenericIterator& operator++() noexcept { ++m_ptr; return *this; }
		GenericIterator& operator--() noexcept { --m_ptr; return *this; }

		GenericIterator operator++(int) noexcept { GenericIterator tmp = *this; ++(*this); return tmp; }
		GenericIterator operator--(int) noexcept { GenericIterator tmp = *this; --(*this); return tmp; }

		// ==================== Arithmetic ====================
		GenericIterator& operator+=(difference_type n) noexcept { m_ptr += n; return *this; }
		GenericIterator& operator-=(difference_type n) noexcept { m_ptr -= n; return *this; }

		friend GenericIterator operator+(GenericIterator it, difference_type n) noexcept { it += n; return it; }
		friend GenericIterator operator+(difference_type n, GenericIterator it) noexcept { return it + n; }
		friend GenericIterator operator-(GenericIterator it, difference_type n) noexcept { it -= n; return it; }

		friend difference_type operator-(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return lhs.m_ptr - rhs.m_ptr; }

		// ==================== Comparison ====================
		friend bool operator==(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return lhs.m_ptr == rhs.m_ptr; }
		friend bool operator!=(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return lhs.m_ptr != rhs.m_ptr; }

		friend bool operator<(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return lhs.m_ptr < rhs.m_ptr; }
		friend bool operator>(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return rhs < lhs; }

		friend bool operator<=(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return !(rhs < lhs); }
		friend bool operator>=(const GenericIterator& lhs, const GenericIterator& rhs) noexcept { return !(lhs < rhs); }

	private:
		pointer m_ptr;
	};

}