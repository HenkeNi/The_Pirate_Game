#pragma once
#include "json_fwd.h"
#include "engine/utils/iterators/generic_iterator.hpp"
#include <functional>
#include <vector>

namespace cursed_engine
{
	// Could try the templated approach, but construct a vector of T and pass that to JsonArrayView
	// use void* and sizse in jsonarrayview to hide implementation? or try to preconstruct vector of T and pass that?

	// Todo; create shared iteraor here...

	class JsonValue;

	class JsonArrayView
	{
	public:
		JsonArrayView(JsonValue& value); // mark as explicit?

		class Iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = rapidjson::Value;
			using difference_type = std::ptrdiff_t;
			using pointer = rapidjson::Value*;
			using reference = rapidjson::Value&;

			// TODO; mark constexpr or not?

			// ==================== Constructors ====================
			Iterator(rapidjson::Value* ptr, std::size_t index) noexcept 
				: m_ptr{ ptr }, m_index{ index }
			{}

			// ==================== Dereference ====================
			JsonValue operator*() const noexcept; // return json value?
			JsonValue operator->() const noexcept;
			JsonValue operator[](difference_type n) const noexcept;

			// ==================== Increment / Decrement ====================
			Iterator& operator++() noexcept;
			Iterator& operator--() noexcept;

			Iterator operator++(int) noexcept;
			Iterator operator--(int) noexcept;

			// ==================== Arithmetic ====================
			Iterator& operator+=(difference_type n) noexcept;
			Iterator& operator-=(difference_type n) noexcept;

			friend Iterator operator+(Iterator it, difference_type n) noexcept;
			friend Iterator operator+(difference_type n, Iterator it) noexcept;
			friend Iterator operator-(Iterator it, difference_type n) noexcept;

			friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) noexcept;

			// ==================== Comparison ====================
			friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept;
			friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept;

			friend bool operator<(const Iterator& lhs, const Iterator& rhs) noexcept;
			friend bool operator>(const Iterator& lhs, const Iterator& rhs) noexcept;

			friend bool operator<=(const Iterator& lhs, const Iterator& rhs) noexcept;
			friend bool operator>=(const Iterator& lhs, const Iterator& rhs) noexcept;

		private:
			pointer m_ptr;
			std::size_t m_index;
		};

		std::size_t size() const; // noexcept?
		[[nodiscard]] bool empty() const noexcept;

		// push back
		// emplace back
		// remove 
		// clear?
		
		Iterator begin() noexcept;
		Iterator end() noexcept;

	private:

		JsonValue& m_value;	
	};

	class JsonArrayConstView
	{
	public:
		JsonArrayConstView(const JsonValue& value);

		class Iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = rapidjson::Value;
			using difference_type = std::ptrdiff_t;
			using pointer = rapidjson::Value*;
			using reference = rapidjson::Value&;

			// TODO; mark constexpr or not?

			// ==================== Constructors ====================
			Iterator(const pointer ptr, std::size_t index) noexcept
				: m_ptr{ ptr }, m_index{ index }
			{
			}

			// ==================== Dereference ====================
			JsonValue operator*() const noexcept; // return json value?
			JsonValue operator->() const noexcept;
			JsonValue operator[](difference_type n) const noexcept;

			// ==================== Increment / Decrement ====================
			Iterator& operator++() noexcept;
			Iterator& operator--() noexcept;

			Iterator operator++(int) noexcept;
			Iterator operator--(int) noexcept;

			// ==================== Arithmetic ====================
			Iterator& operator+=(difference_type n) noexcept;
			Iterator& operator-=(difference_type n) noexcept;

			friend Iterator operator+(Iterator it, difference_type n) noexcept;
			friend Iterator operator+(difference_type n, Iterator it) noexcept;
			friend Iterator operator-(Iterator it, difference_type n) noexcept;

			friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) noexcept;

			// ==================== Comparison ====================
			friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept;
			friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept;

			friend bool operator<(const Iterator& lhs, const Iterator& rhs) noexcept;
			friend bool operator>(const Iterator& lhs, const Iterator& rhs) noexcept;

			friend bool operator<=(const Iterator& lhs, const Iterator& rhs) noexcept;
			friend bool operator>=(const Iterator& lhs, const Iterator& rhs) noexcept;

		private:
			const pointer m_ptr;
			std::size_t m_index;
		};

		[[nodiscard]] std::size_t size() const; // noexcept?
	

		Iterator begin() noexcept; // const iterator instead?
		Iterator end() noexcept;

	private:
		const JsonValue& m_value;
	};



	class JsonValue
	{
	private:
		using PropertyCallback = std::function<void(const char*, JsonValue)>; // accept ref? maybe const overload?

	public:
		explicit JsonValue(rapidjson::Value* value); // accept reference instead? or represent invalid json vale object
		~JsonValue();

		[[nodiscard]] bool isNull() const noexcept;
		[[nodiscard]] bool isObject() const noexcept;
		[[nodiscard]] bool isArray() const noexcept;
		[[nodiscard]] bool isBool() const noexcept;
		[[nodiscard]] bool isDouble() const noexcept;
		[[nodiscard]] bool isInt() const noexcept;
		[[nodiscard]] bool isString() const noexcept;

		[[nodiscard]] bool asBool() const;
		[[nodiscard]] int asInt() const;
		[[nodiscard]] double asDouble() const;
		[[nodiscard]] const char* asString() const; // TODO; return string view? or string?
		[[nodiscard]] JsonArrayView asArray() noexcept;
		[[nodiscard]] JsonArrayConstView asArray() const noexcept; // return value as const?

		//JsonObjectView asObject() noexcept;

		// Array operations
		[[nodiscard]] size_t size() const;
		JsonValue operator[](size_t index) noexcept; // prevents warnings? to not use nodiscard here
		[[nodiscard]] JsonValue operator[](size_t index) const;

		// Object operations
		[[nodiscard]] bool has(const char* key) const;
		JsonValue operator[](const char* key);
		[[nodiscard]] JsonValue operator[](const char* key) const; // or string view?

		void forEachProperty(const PropertyCallback& callback) const;

		// Conversion
		//std::string toString(bool pretty = false) const;

	private:
		friend class JsonArrayView;
		friend class JsonArrayConstView;

		rapidjson::Value* m_value;
	};
}