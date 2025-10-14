#pragma once
#include "engine/core/logger.h"
#include <memory>
#include <filesystem>
#include <string_view>
#include <rapidjson/document.h>

namespace cursed_engine
{
	// TODO; use PIMPL if wanting to hide rapidjson::Document in the future! (remove include)
	// TODO; either create separate JsonReader/JsonWriter or as the same class
	// Rename file json.h? or json_document.h?

	namespace fs = std::filesystem;

	template <typename T>
	struct TypeHelper;
	//{
	//	//static_assert(sizeof(T) == 0, "TypeHelper not implemented for this type");
	//};

	template <>
	struct TypeHelper<int>
	{
		[[nodiscard]] inline static bool is(const rapidjson::Value& val) noexcept { return val.IsInt(); }
		[[nodiscard]] inline static int get(const rapidjson::Value& val) noexcept { return val.GetInt(); }
	};

	template <>
	struct TypeHelper<bool>
	{
		static bool is(const rapidjson::Value& val) { return val.IsBool(); }
		static bool get(const rapidjson::Value& val) { return val.GetBool(); }
	};

	template <>
	struct TypeHelper<float>
	{
		static bool is(const rapidjson::Value& val) { return val.IsFloat(); }
		static float get(const rapidjson::Value& val) { return val.GetFloat(); }
	};

	template <>
	struct TypeHelper<double>
	{
		static bool is(const rapidjson::Value& val) { return val.IsDouble(); }
		static double get(const rapidjson::Value& val) { return val.GetDouble(); }
	};

	template <>
	struct TypeHelper<std::string>
	{
		static bool is(const rapidjson::Value& val) { return val.IsString(); }
		static std::string get(const rapidjson::Value& val) { return val.GetString(); }
	};

	struct JsonResult
	{
		bool success;
		std::string errorMessage;
	};

	class JsonHandler
	{
	public:
		JsonHandler();
		JsonHandler(const fs::path& path);

		[[nodiscard]] JsonResult loadFromFile(const fs::path& path);

		template <typename T>
		[[nodiscard]] T getValueOr(const char* key, const T& defaultValue);

		[[nodiscard]] std::optional<rapidjson::Value::Object> getObject(const char* key);

		[[nodiscard]] inline bool isLoaded() const { return m_isLoaded; }
		// TODO; create write functions!?

	private:
		rapidjson::Document m_document;
		bool m_isLoaded;
	};

	template <typename T>
	T JsonHandler::getValueOr(const char* key, const T& defaultValue)
	{
		constexpr std::string_view logPrefix = "[JsonParser::getValueOr] - ";

		if (!m_isLoaded) [[likely]]
		{
			Logger::logError(std::string(logPrefix) + "Document not loaded!");
			return defaultValue;
		}

		if (!key || !m_document.HasMember(key))
		{
			Logger::logError(std::string(logPrefix) + "Missing key: " + (key ? key : "<null>"));
			return defaultValue;
		}

		const auto& val = m_document[key];

		if (!TypeHelper<T>::Is(val))
		{
			Logger::logError(std::string(logPrefix) + "Type mismatch for key: " + key);
			return defaultValue;
		}

		return TypeHelper<T>::Get(val);
	}









	class JSONDocument
	{
	public:
		JSONDocument();
		~JSONDocument() = default;

		JSONDocument(const JSONDocument&) = delete;
		JSONDocument& operator=(const JSONDocument&) = delete;

		JSONDocument(JSONDocument&&) = default;
		JSONDocument& operator=(JSONDocument&&) = default;

		JsonResult loadFromFile(const fs::path& path);

		bool saveToFile(const fs::path& path) const
		{
			return true;
		}

		[[nodiscard]] inline const rapidjson::Document& get() const { return m_document; }
		[[nodiscard]] inline rapidjson::Document& get() { return m_document; }

		template <typename T>
		T parseAs() const;


		// TODO; is ValueProxy redundant? maybe just use document["test"]["child"]???

		// Safe value access with chaining
		class ValueProxy
		{
		public:
			ValueProxy(const rapidjson::Value* value)
				: m_value(value)
			{
			}

			// TODO; return ref or copy?
			ValueProxy operator[](const char* key) const
			{
				if (m_value && m_value->IsObject() && m_value->HasMember(key))
				{
					return ValueProxy{ &(*m_value)[key] };
				}

				return ValueProxy(nullptr);
			}

			ValueProxy operator[](size_t index) const
			{
				if (m_value && m_value->IsArray() && index < m_value->Size()) {
					return ValueProxy{ &(*m_value)[index] };
				}

				return ValueProxy(nullptr);
			}

			template<typename T>
			T to(T defaultValue = T{}) const
			{
				if (!m_value) return defaultValue;

				return TypeHelper<T>::get(*m_value);
				// Implementation for type conversion...
			}

			bool isValid() const { return m_value != nullptr; }

		private:
			const rapidjson::Value* m_value;
		};

		ValueProxy operator[](const char* key) const
		{
			assert(m_isLoaded && "Document is not loaded!");

			if (m_document.IsObject() && m_document.HasMember(key))
			{
				return ValueProxy(&m_document[key]);
			}

			return ValueProxy(nullptr);
		}


		//class ValueProxy
		//{
		//public:
		//	ValueProxy(const rapidjson::Value* value)
		//		: m_value{ value }
		//	{
		//	}

		//	// TODO; overload () operator []


		//	[[nodiscard]] bool isValid() const noexcept { return m_value != nullptr; }

		//private:
		//	const rapidjson::Value* m_value;
		//};

	private:
		rapidjson::Document m_document;
		bool m_isLoaded;
	};
}