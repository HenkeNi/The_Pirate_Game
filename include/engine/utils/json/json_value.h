#pragma once
#include <memory>
//#include <rapidjson/fwd.h>

#include <functional>
#include "json_fwd.h"

// put in json_fwd.h?
//namespace rapidjson
//{
//	class CrtAllocator;
//
//	template<typename CharType>
//	struct UTF8;
//
//	template <typename BaseAllocator>
//	class MemoryPoolAllocator;
//
//	template <typename Encoding, typename Allocator>
//	class GenericValue;
//
//	typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator> > Value;
//}

namespace cursed_engine
{
	class JsonObject
	{

	};

	class JsonValue
	{
	public:
		explicit JsonValue(rapidjson::Value* value); // accept JsonDocumentImpl?
		~JsonValue();

		[[nodiscard]] bool isNull() const noexcept;
		[[nodiscard]] bool isObject() const noexcept;
		[[nodiscard]] bool isArray() const noexcept;
		[[nodiscard]] bool isBool() const noexcept;
		[[nodiscard]] bool isDouble() const noexcept;
		[[nodiscard]] bool isInt() const noexcept;
		[[nodiscard]] bool isString() const noexcept;

		[[nodiscard]] bool asBool() const;
		[[nodiscard]] double asDouble() const;
		[[nodiscard]] int asInt() const;
		[[nodiscard]] const char* asString() const; // return string view?

		// Array operations
		size_t size() const;
		JsonValue operator[](size_t index) const; // shouldnt this return an array of T?

		// Object operations
		bool has(const char* key) const;
		JsonValue operator[](const char* key) const; // or stirng view?

		// JsonValue get(const char*); // ???

		// TEMP:
		void forEachArray(const std::function<void(JsonValue value)>& func) const; // rename forEachElement?
		void forEachObject(const std::function<void(const char*, JsonValue value)>& func) const; // rename forEachProperty?!
		//void forEach(const std::function<void(const char*, const rapidjson::Value& value)>& func);

		// Iteration
		//class Iterator;  // For range-based for loops

		// Conversion
		//std::string toString(bool pretty = false) const;

	private:
		rapidjson::Value* m_value; // store void ptr? // store generic object instead?		 
	};
}