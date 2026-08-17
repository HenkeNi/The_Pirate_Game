#pragma once
#include "engine/utils/concepts.h"
#include "engine/utils/id_generator.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <typeindex>

namespace cursed_engine
{
	template <typename Value, typename Key = uint32_t>
	class Registry
	{
	public:

		// why entry here???
		template <typename Entry, typename... Args> // FInd better name than Entry? maybe class should use Entry instead of T?
		Value& emplace(std::string name, Key key, Args&&... args);

		void insert(Value entry);

		[[nodiscard]] const Value& get(const char* name) const;
		
		[[nodiscard]] const Value* tryGet(const char* name) const;

		// getByname and get by id?!

		[[nodiscard]] size_t size() const;

		[[nodiscard]] bool isValid(Key key) const;

		[[nodiscard]] bool isValid(const std::string& name) const;

		void clear();

	private:
		struct Tag {};

		std::vector<Value> m_entries;
			
		//std::unordered_map<std::string, Id> m_namesToIds; // or name to inde?
		std::unordered_map<std::string, std::size_t> m_namesToIndexes; // or name to inde?
		std::unordered_map<Key, std::size_t> m_keysToIndexes; // use sparse set?

		// mutex?
	};

#pragma region Definitions

	template <typename T, typename Id>
	template <typename Entry, typename... Args>
	T& Registry<T, Id>::emplace(std::string name, Id id, Args&&... args)
	{
		auto& entry = m_entries.emplace_back(std::forward<Args>(args)...);

		std::size_t index = m_entries.size() - 1;

		m_keysToIndexes.insert({ id, index });
		m_namesToIndexes.insert({ name, index });
		
		return m_entries.back(); // correct?
	}

	template <typename T, typename Id>
	void Registry<T, Id>::insert(T entry)
	{
		m_entries.push_back(std::move(entry));
	}

	template <typename T, typename Id>
	const T& Registry<T, Id>::get(const char* name) const
	{
		auto index = m_namesToIndexes.at(name);
		return m_entries.at(index);
	}

	template <typename T, typename Id>
	const T* Registry<T, Id>::tryGet(const char* name) const
	{
		return nullptr;
	}

	template <typename T, typename Id>
	size_t Registry<T, Id>::size() const
	{
		return 0;
	}

	template <typename T, typename Id>
	bool Registry<T, Id>::isValid(Id id) const
	{
		return false;
	}

	template <typename T, typename Id>
	bool Registry<T, Id>::isValid(const std::string& name) const
	{
		return m_namesToIndexes.contains(name);
	}

	template <typename T, typename Id>
	void Registry<T, Id>::clear()
	{
		m_entries.clear();
		m_namesToIndexes.clear();
		m_keysToIndexes.clear();
	}

#pragma endregion
}