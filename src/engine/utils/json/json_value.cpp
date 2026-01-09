#include "engine/utils/json/json_value.h"
#include <rapidjson/document.h>
#include <cassert>

namespace cursed_engine
{
	//struct JsonValue::Impl
	//{
	//	rapidjson::Value* value;
	//};

	JsonValue::JsonValue(rapidjson::Value* value)
		: m_value{ value }
		//: m_impl{ std::make_unique<JsonValue::Impl>(value) }
	{
	}

	JsonValue::~JsonValue() = default;

	bool JsonValue::isNull() const noexcept
	{
		return !m_value;
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

	double JsonValue::asDouble() const
	{
		return m_value->GetDouble();
	}

	int JsonValue::asInt() const
	{
		return m_value->GetInt();
	}

	const char* JsonValue::asString() const
	{
		return m_value->GetString();
	}

	size_t JsonValue::size() const
	{
		return m_value->Size();
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

	JsonValue JsonValue::operator[](const char* key) const
	{
		if (!m_value || !m_value->IsObject() || !m_value->HasMember(key))
			return JsonValue{ nullptr };

		return JsonValue{ &(*m_value)[key] };
	}

	void JsonValue::forEachArray(const std::function<void(JsonValue value)>& func) const
	{
		assert(m_value->IsArray() && "Value is not an array!");

		for (auto& value : m_value->GetArray()) // use const? if so, store rapidjson as const member in class?
		{
			func(JsonValue{ &value });
		}
	}

	void JsonValue::forEachObject(const std::function<void(const char* name, JsonValue value)>& func) const
	{
		assert(m_value->IsObject() && "Value is not an object!");

		for (auto& [name, value] : m_value->GetObject())
		{
			func(name.GetString(), JsonValue{ &value }); // Pass copy or const ref? - prefer copy!?
		}
	}

	//void JsonValue::forEach(const std::function<void(const char*, JsonValue value)>& func) const
	//{
	//	// TODO; Fix!

	//	if (m_value->IsArray())
	//	{
	//		//for (const auto& value : m_value->GetArray())
	//		for (auto& value : m_value->GetArray()) // use const? if so, store rapidjson as const member in class?
	//		{
	//			func("", JsonValue{ &value });
	//		}
	//	}
	//	else if (m_value->IsObject())
	//	{
	//		//for (const auto& [name, value] : m_value->GetObject())
	//		for (auto& [name, value] : m_value->GetObject())
	//		{
	//			func(name.GetString(), JsonValue{ &value }); // Pass copy or const ref? - prefer copy!?
	//		}
	//	}

	//}

	//void JsonValue::initialize(rapidjson::Value* value)
	//{
	//	m_value = value;
	//}
}