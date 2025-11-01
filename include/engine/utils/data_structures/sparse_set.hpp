#pragma once
#include "engine/utils/type_traits.h"
#include <cassert>
#include <limits>
#include <stddef.h>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace cursed_engine
{
	// TODO; use dynamic_array instead?

	template <typename Value, Integral Key = std::size_t>
	class sparse_set final
	{
	private:
		template <typename T>
		class iterator_impl;

	public:
		// ==================== Type Aliases ====================
		using iterator = iterator_impl<Value>;
		using const_iterator = iterator_impl<const Value>;

		using reverse_iterator = std::reverse_iterator<iterator_impl<Value>>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// ==================== Construction, Assignment & Destruction ====================
		constexpr sparse_set() = default;
		explicit sparse_set(std::size_t capacity);

		sparse_set(const sparse_set&) = default;
		sparse_set(sparse_set&&) noexcept = default;

		sparse_set& operator=(const sparse_set&) = default;
		sparse_set& operator=(sparse_set&&) noexcept = default;

		~sparse_set() = default;

		// ==================== Iterators ====================
		constexpr iterator begin() noexcept { return iterator{ m_dense.data() }; }
		constexpr iterator end() noexcept { return iterator{ m_dense.data() + m_dense.size() }; }

		constexpr const_iterator begin() const noexcept { return const_iterator{ m_dense.data() }; }
		constexpr const_iterator end()  const noexcept { return const_iterator{ m_dense.data() + m_dense.size() }; }

		constexpr const_iterator cbegin() const noexcept { return const_iterator{ m_dense.data() }; }
		constexpr const_iterator cend() const noexcept { return const_iterator{ m_dense.data() + m_dense.size() }; }

		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{ end() }; }
		constexpr reverse_iterator rend() noexcept { return reverse_iterator{ begin() }; }

		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{ end() }; }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{ begin() }; }

		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{ end() }; }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ begin() }; }

		// ==================== Element Access ====================
		// Safe access (checked)
		const Value& at(Key key) const;
		Value& at(Key key);

		// Unsafe access (unchecked)
		const Value& operator[](Key key) const;
		Value& operator[](Key key);

		// Pointer access
		const Value* get(Key key) const noexcept;
		Value* get(Key key) noexcept;

		// ==================== Modifiers ====================
		// Insertion
		template <typename... Args>
		std::pair<iterator, bool> emplace(Key key, Args&&... args);
		void insert(Key key, Value&& value);

		// Removal
		bool erase(Key key);
		void clear() noexcept;

		// Memory management
		void reserve(std::size_t capacity);
		void shrink_to_fit();
		void swap(sparse_set& other) noexcept;

		// ==================== Capacity ====================
		[[nodiscard]] constexpr bool empty() const noexcept;
		[[nodiscard]] bool contains(Key key) const;
		[[nodiscard]] constexpr std::size_t size() const noexcept;

	private:
		template <typename T>
		class iterator_impl
		{
		public:
			// ==================== Type Aliases ====================
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using reference = T&;
			using pointer = T*;

			// ==================== Constructors ====================
			constexpr iterator_impl() noexcept : m_ptr{ nullptr } {}
			constexpr explicit iterator_impl(pointer ptr) noexcept : m_ptr{ ptr } {}

			// ==================== Dereference ====================
			constexpr reference operator*() const noexcept { return *m_ptr; }
			constexpr pointer operator->() const noexcept { return m_ptr; }
			constexpr reference operator[](difference_type n) const noexcept { return *(m_ptr + n); }

			// ==================== Increment / Decrement ====================
			constexpr iterator_impl& operator++() noexcept { ++m_ptr; return *this; }
			constexpr iterator_impl& operator--() noexcept { --m_ptr; return *this; }

			constexpr iterator_impl operator++(int) noexcept { iterator_impl tmp = *this; ++(*this); return tmp; }
			constexpr iterator_impl operator--(int) noexcept { iterator_impl tmp = *this; --(*this); return tmp; }

			// ==================== Arithmetic ====================
			constexpr iterator_impl& operator+=(difference_type n) noexcept { m_ptr += n; return *this; }
			constexpr iterator_impl& operator-=(difference_type n) noexcept { m_ptr -= n; return *this; }

			friend constexpr iterator_impl operator+(iterator_impl it, difference_type n) noexcept { it += n; return it; }
			friend constexpr iterator_impl operator+(difference_type n, iterator_impl it) noexcept { return it + n; }
			friend constexpr iterator_impl operator-(iterator_impl it, difference_type n) noexcept { it -= n; return it; }

			friend constexpr difference_type operator-(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return lhs.m_ptr - rhs.m_ptr; }

			// ==================== Comparison ====================
			friend constexpr bool operator==(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return lhs.m_ptr == rhs.m_ptr; }
			friend constexpr bool operator!=(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return lhs.m_ptr != rhs.m_ptr; }

			friend constexpr bool operator<(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return lhs.m_ptr < rhs.m_ptr; }
			friend constexpr bool operator>(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return rhs < lhs; }

			friend constexpr bool operator<=(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return !(rhs < lhs); }
			friend constexpr bool operator>=(const iterator_impl& lhs, const iterator_impl& rhs) noexcept { return !(lhs < rhs); }

		private:
			pointer m_ptr;
		};

		// ==================== Data Members ====================
		inline static constexpr Key INVALID_INDEX = std::numeric_limits<Key>::max();

		std::vector<Key> m_sparse;				 // maps key => index into dense (invalid_index = empty)
		std::vector<std::size_t> m_reverse;		 // maps dense index => key
		std::vector<Value> m_dense;				 // actual values
	};

	template <typename Value, typename Key = std::size_t>
	using SparseSet = sparse_set<Value, Key>;

#pragma region Methods

	template <typename Value, Integral Key>
	sparse_set<Value, Key>::sparse_set(std::size_t capacity)
	{
		m_sparse.resize(capacity, INVALID_INDEX);
		m_reverse.resize(capacity, INVALID_INDEX);

		m_dense.reserve(capacity);
	}

	template <typename Value, Integral Key>
	const Value& sparse_set<Value, Key>::at(Key key) const
	{
		if (!contains(key)) throw std::out_of_range("Invalid key");
		return m_dense[m_sparse[key]];
	}

	template <typename Value, Integral Key>
	Value& sparse_set<Value, Key>::at(Key key)
	{
		return const_cast<Value&>(std::as_const(*this).at(key));
	}

	template <typename Value, Integral Key>
	const Value& sparse_set<Value, Key>::operator[](Key key) const
	{
		assert(contains(key));
		return m_dense[m_sparse[key]];
	}

	template <typename Value, Integral Key>
	Value& sparse_set<Value, Key>::operator[](Key key)
	{
		assert(contains(key));
		return m_dense[m_sparse[key]];
	}

	template <typename Value, Integral Key>
	const Value* sparse_set<Value, Key>::get(Key key) const noexcept
	{
		return contains(key) ? &m_dense[m_sparse[key]] : nullptr;
	}

	template <typename Value, Integral Key>
	Value* sparse_set<Value, Key>::get(Key key) noexcept
	{
		return contains(key) ? &m_dense[m_sparse[key]] : nullptr;
	}

	template <typename Value, Integral Key>
	template <typename... Args>
	auto sparse_set<Value, Key>::emplace(Key key, Args&&... args) -> std::pair<iterator, bool>
	{
		// Early exit for invalid key
		if (key < 0 || key == INVALID_INDEX)
		{
			return { iterator{ m_dense.data() + m_dense.size() }, false };
		}

		// Key already exists: overwrite existing value
		if (contains(key))
		{
			auto index = m_sparse[key];
			m_dense[index] = Value(std::forward<Args>(args)...);

			return { iterator{ m_dense.data() + index }, true };
		}

		// Resize sparse/reverse vectors if necessary
		if (static_cast<std::size_t>(key) >= m_sparse.size())
		{
			const std::size_t newSize = std::max(static_cast<std::size_t>(key) + 1, m_sparse.size() * 2);

			m_sparse.resize(newSize, INVALID_INDEX);
			m_reverse.resize(newSize);
		}

		const Key index = static_cast<Key>(m_dense.size());

		m_sparse[key] = index;
		m_reverse[index] = key;

		m_dense.emplace_back(std::forward<Args>(args)...);
		return { iterator{ m_dense.data() + index }, true };
	}

	template <typename Value, Integral Key>
	void sparse_set<Value, Key>::insert(Key key, Value&& value)
	{
		emplace(key, std::forward<Value>(value));
	}

	template <typename Value, Integral Key>
	bool sparse_set<Value, Key>::erase(Key key)
	{
		if (!contains(key))
			return false;

		const Key indexToRemove = m_sparse[key];
		const std::size_t lastIndex = m_dense.size() - 1;

		if (indexToRemove != static_cast<Key>(lastIndex))
		{
			m_dense[indexToRemove] = std::move(m_dense[lastIndex]);

			// Update the moved element's sparse index
			const Key lastKey = m_reverse[lastIndex];
			m_sparse[lastKey] = indexToRemove;
			m_reverse[indexToRemove] = lastKey;
		}

		m_sparse[key] = INVALID_INDEX;
		m_dense.pop_back();

		return true;
	}

	template <typename Value, Integral Key>
	void sparse_set<Value, Key>::clear() noexcept
	{
		m_dense.clear();
		std::fill(m_sparse.begin(), m_sparse.end(), INVALID_INDEX);
		std::fill(m_reverse.begin(), m_reverse.end(), INVALID_INDEX);
	}

	template <typename Value, Integral Key>
	void sparse_set<Value, Key>::reserve(std::size_t capacity)
	{
		m_sparse.reserve(capacity);
		m_reverse.reserve(capacity);
		m_dense.reserve(capacity);
	}

	template <typename Value, Integral Key>
	void sparse_set<Value, Key>::shrink_to_fit()
	{
		m_sparse.shrink_to_fit();
		m_reverse.shrink_to_fit();
		m_dense.shrink_to_fit();
	}

	template <typename Value, Integral Key>
	void sparse_set<Value, Key>::swap(sparse_set& other) noexcept
	{
		m_sparse.swap(other.m_sparse);
		m_reverse.swap(other.m_reverse);
		m_dense.swap(other.m_dense);
	}

	template <typename Value, Integral Key>
	constexpr bool sparse_set<Value, Key>::empty() const noexcept
	{
		return m_dense.empty();
	}

	template <typename Value, Integral Key>
	bool sparse_set<Value, Key>::contains(Key key) const
	{
		return key >= 0 && static_cast<std::size_t>(key) < m_sparse.size() && m_sparse[key] != INVALID_INDEX;
	}

	template <typename Value, Integral Key>
	constexpr std::size_t sparse_set<Value, Key>::size() const noexcept
	{
		return m_dense.size();
	}

#pragma endregion
}