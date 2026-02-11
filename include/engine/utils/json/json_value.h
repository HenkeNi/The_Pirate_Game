#pragma once
#include "json_fwd.h"
#include <functional>

namespace cursed_engine
{
	class JsonValue
	{
	public:
		explicit JsonValue(rapidjson::Value* value);
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
		[[nodiscard]] const char* asString() const; // TODO; return string view?

		// Array operations
		[[nodiscard]] size_t size() const;
		JsonValue operator[](size_t index) const; // shouldnt this return an array of T?

		// Object operations
		[[nodiscard]] bool has(const char* key) const;
		JsonValue operator[](const char* key) const; // or stirng view?

		// TEMP:
		void forEachArray(const std::function<void(JsonValue value)>& func) const; // rename forEachElement?
		void forEachObject(const std::function<void(const char*, JsonValue value)>& func) const; // rename forEachProperty?! - needed??
		
		// Iteration
		//class Iterator;  // For range-based for loops

		// Conversion
		//std::string toString(bool pretty = false) const;

	private:
		rapidjson::Value* m_value;		 
	};
}