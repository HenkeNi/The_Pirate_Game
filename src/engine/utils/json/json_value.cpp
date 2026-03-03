#include "engine/utils/json/json_value.h"
#include <rapidjson/document.h>
#include <cassert>

namespace cursed_engine
{

	JsonArrayView::JsonArrayView(JsonValue& value)
		: m_value{ value }
	{
	}

	std::size_t JsonArrayView::size() const
	{
		return m_value.size();
	}

	bool JsonArrayView::empty() const noexcept
	{
		return m_value.size() == 0; // TODO; add empty
	}


	/*JsonArrayView::iterator(JsonValue* value)
	: JsonArrayView::BaseType{ value }
	{
	}*/


	//JsonArrayView::iterator::iterator(JsonValue* value)
	//	: BaseType{ value->m_value->GetArray() }
	//{
	//}

	JsonValue JsonArrayView::Iterator::operator*() const noexcept
	{
		return JsonValue{ &m_ptr->GetArray()[m_index] };
		//return JsonValue{ m_ptr };
	}

	JsonValue JsonArrayView::Iterator::operator->() const noexcept
	{
		return JsonValue{ &m_ptr->GetArray()[m_index] };
		//return JsonValue{ m_ptr };
	}

	JsonValue JsonArrayView::Iterator::operator[](difference_type n) const noexcept
	{
		return JsonValue{ &m_ptr->GetArray()[n] };
		//return JsonValue{ &m_ptr[n] };
	}

	JsonArrayView::Iterator& JsonArrayView::Iterator::operator++() noexcept
	{
		++m_index; return *this;
		//++m_ptr; return *this;
	}

	JsonArrayView::Iterator& JsonArrayView::Iterator::operator--() noexcept
	{
		--m_index; return *this;
		//--m_ptr; return *this;
	}

	JsonArrayView::Iterator JsonArrayView::Iterator::operator++(int) noexcept
	{
		Iterator tmp = *this; ++(*this); return tmp;
	}

	JsonArrayView::Iterator JsonArrayView::Iterator::operator--(int) noexcept
	{
		Iterator tmp = *this; --(*this); return tmp;
	}

	JsonArrayView::Iterator& JsonArrayView::Iterator::operator+=(JsonArrayView::Iterator::difference_type n) noexcept
	{
		m_index += n; return *this;
		//m_ptr += n; return *this;
	}

	JsonArrayView::Iterator& JsonArrayView::Iterator::operator-=(JsonArrayView::Iterator::difference_type n) noexcept
	{
		m_index -= n; return *this;
		//m_ptr -= n; return *this;
	}

	JsonArrayView::Iterator operator+(JsonArrayView::Iterator it, JsonArrayView::Iterator::difference_type n) noexcept
	{
		it += n; return it;
	}

	JsonArrayView::Iterator operator+(JsonArrayView::Iterator::difference_type n, JsonArrayView::Iterator it) noexcept
	{
		return it + n;
	}

	JsonArrayView::Iterator operator-(JsonArrayView::Iterator it, JsonArrayView::Iterator::difference_type n) noexcept
	{
		it -= n; return it;
	}

	JsonArrayView::Iterator::difference_type operator-(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return lhs.m_index - rhs.m_index;
		//return lhs.m_ptr - rhs.m_ptr;
	}

	bool operator==(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return (lhs.m_index == rhs.m_index && lhs.m_ptr == rhs.m_ptr);
		//return lhs.m_ptr == rhs.m_ptr;
	}

	bool operator!=(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return lhs.m_index != rhs.m_index; // Check value is the same?
	}

	bool operator<(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return lhs.m_index < rhs.m_index;
		//return lhs.m_ptr < rhs.m_ptr;
	}

	bool operator>(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return rhs < lhs;
	}

	bool operator<=(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return !(rhs < lhs);
	}

	bool operator>=(const JsonArrayView::Iterator& lhs, const JsonArrayView::Iterator& rhs) noexcept
	{
		return !(lhs < rhs);
	}



	JsonArrayView::Iterator JsonArrayView::begin() noexcept
	{
		return JsonArrayView::Iterator{ m_value.m_value, 0 };  // TODO; use array insteaD??
		//return JsonArrayView::iterator{ &m_value }; 
	}

	JsonArrayView::Iterator JsonArrayView::end() noexcept
	{
		//return JsonArrayView::iterator{ &m_value.m_value->GetArray() + size()};
		return JsonArrayView::Iterator{ m_value.m_value, size() };
	}



	JsonArrayConstView::JsonArrayConstView(const JsonValue& value)
		: m_value{ value }
	{
	}


	JsonValue JsonArrayConstView::Iterator::operator*() const noexcept
	{
		return JsonValue{ &m_ptr->GetArray()[m_index] };
		//return JsonValue{ m_ptr };
	}

	JsonValue JsonArrayConstView::Iterator::operator->() const noexcept
	{
		return JsonValue{ &m_ptr->GetArray()[m_index] };
		//return JsonValue{ m_ptr };
	}

	JsonValue JsonArrayConstView::Iterator::operator[](difference_type n) const noexcept
	{
		return JsonValue{ &m_ptr->GetArray()[n] };
		//return JsonValue{ &m_ptr[n] };
	}

	JsonArrayConstView::Iterator& JsonArrayConstView::Iterator::operator++() noexcept
	{
		++m_index; return *this;
		//++m_ptr; return *this;
	}

	JsonArrayConstView::Iterator& JsonArrayConstView::Iterator::operator--() noexcept
	{
		--m_index; return *this;
		//--m_ptr; return *this;
	}

	JsonArrayConstView::Iterator JsonArrayConstView::Iterator::operator++(int) noexcept
	{
		Iterator tmp = *this; ++(*this); return tmp;
	}

	JsonArrayConstView::Iterator JsonArrayConstView::Iterator::operator--(int) noexcept
	{
		Iterator tmp = *this; --(*this); return tmp;
	}

	JsonArrayConstView::Iterator& JsonArrayConstView::Iterator::operator+=(JsonArrayConstView::Iterator::difference_type n) noexcept
	{
		m_index += n; return *this;
		//m_ptr += n; return *this;
	}

	JsonArrayConstView::Iterator& JsonArrayConstView::Iterator::operator-=(JsonArrayConstView::Iterator::difference_type n) noexcept
	{
		m_index -= n; return *this;
		//m_ptr -= n; return *this;
	}

	JsonArrayConstView::Iterator operator+(JsonArrayConstView::Iterator it, JsonArrayConstView::Iterator::difference_type n) noexcept
	{
		it += n; return it;
	}

	JsonArrayConstView::Iterator operator+(JsonArrayConstView::Iterator::difference_type n, JsonArrayConstView::Iterator it) noexcept
	{
		return it + n;
	}

	JsonArrayConstView::Iterator operator-(JsonArrayConstView::Iterator it, JsonArrayConstView::Iterator::difference_type n) noexcept
	{
		it -= n; return it;
	}

	JsonArrayConstView::Iterator::difference_type operator-(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return lhs.m_index - rhs.m_index;
		//return lhs.m_ptr - rhs.m_ptr;
	}

	bool operator==(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return (lhs.m_index == rhs.m_index && lhs.m_ptr == rhs.m_ptr);
		//return lhs.m_ptr == rhs.m_ptr;
	}

	bool operator!=(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return lhs.m_index != rhs.m_index; // Check value is the same?
	}

	bool operator<(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return lhs.m_index < rhs.m_index;
		//return lhs.m_ptr < rhs.m_ptr;
	}

	bool operator>(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return rhs < lhs;
	}

	bool operator<=(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return !(rhs < lhs);
	}

	bool operator>=(const JsonArrayConstView::Iterator& lhs, const JsonArrayConstView::Iterator& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	std::size_t JsonArrayConstView::size() const
	{
		return m_value.size();
	}

	JsonArrayConstView::Iterator JsonArrayConstView::begin() noexcept
	{
		return JsonArrayConstView::Iterator{ m_value.m_value, 0 };  // TODO; use array insteaD??
		//return JsonArrayView::iterator{ &m_value }; 
	}

	JsonArrayConstView::Iterator JsonArrayConstView::end() noexcept
	{
		//return JsonArrayView::iterator{ &m_value.m_value->GetArray() + size()};
		return JsonArrayConstView::Iterator{ m_value.m_value, size() };
	}










	JsonValue::JsonValue(rapidjson::Value* value)
		: m_value{ value }
	{
	}

	JsonValue::~JsonValue() = default;

	bool JsonValue::isNull() const noexcept
	{
		return m_value->IsNull();
	}

	bool JsonValue::isObject() const noexcept
	{
		return m_value->IsObject();
	}

	bool JsonValue::isArray() const noexcept
	{
		return m_value->IsArray();
	}

	bool JsonValue::isBool() const noexcept
	{
		return m_value->IsBool();
	}

	bool JsonValue::isDouble() const noexcept
	{
		return m_value->IsDouble();
	}

	bool JsonValue::isInt() const noexcept
	{
		return m_value->IsInt();
	}

	bool JsonValue::isString() const noexcept
	{
		return m_value->IsString();
	}

	bool JsonValue::asBool() const
	{
		return m_value->GetBool();
	}

	int JsonValue::asInt() const
	{
		return m_value->GetInt();
	}

	double JsonValue::asDouble() const
	{
		return m_value->GetDouble();
	}

	const char* JsonValue::asString() const
	{
		return m_value->GetString();
	}

	JsonArrayView JsonValue::asArray() noexcept
	{
		return JsonArrayView(*this);
	}

	JsonArrayConstView JsonValue::asArray() const noexcept
	{
		return JsonArrayConstView(*this);
	}

	/*JsonObjectView JsonValue::asObject() noexcept
	{
		return JsonObjectView(*this);
	}*/

	size_t JsonValue::size() const
	{
		return m_value->Size();
	}

	JsonValue JsonValue::operator[](size_t index) noexcept
	{
		// TODO; use const cast?

		if (!m_value || !m_value->IsArray() || index >= m_value->Size())
			return JsonValue{ nullptr };

		return JsonValue{ &(*m_value)[index] };
	}

	JsonValue JsonValue::operator[](size_t index) const
	{
		if (!m_value || !m_value->IsArray() || index >= m_value->Size())
			return JsonValue{ nullptr };

		return JsonValue{ &(*m_value)[index] };
	}

	bool JsonValue::has(const char* key) const
	{
		if (!m_value || !m_value->IsObject())
			return false;

		return m_value->HasMember(key);
	}

	JsonValue JsonValue::operator[](const char* key)
	{
		// use const cast instead?
		if (!m_value || !m_value->IsObject() || !m_value->HasMember(key))
			return JsonValue{ nullptr };

		return JsonValue{ &(*m_value)[key] };
	}

	JsonValue JsonValue::operator[](const char* key) const
	{
		if (!m_value || !m_value->IsObject() || !m_value->HasMember(key))
			return JsonValue{ nullptr };

		return JsonValue{ &(*m_value)[key] };
	}

	void JsonValue::forEachProperty(const PropertyCallback& callback) const
	{
		assert(m_value->IsObject() && "forEachProperty failed - json value is not an object!");

		for (auto& [name, value] : m_value->GetObject())
		{
			callback(name.GetString(), JsonValue{ &value }); // Pass copy or const ref? - prefer copy!?
		}
	}
}