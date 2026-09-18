#pragma once
#include <unordered_map>

namespace cursed_engine
{
	template <typename Key, typename Value>
	class Registry
	{
	public:
		template <typename... Args>
		Value& emplace(Key key, Args&&... args);

		void insert(Key key, Value value);
		bool remove(const Key& key);

		[[nodiscard]] const Value& get(const Key& key) const;
		[[nodiscard]] Value& get(const Key& key);

		[[nodiscard]] const Value* tryGet(const Key& key) const;
		[[nodiscard]] Value* tryGet(const Key& key);

		[[nodiscard]] size_t size() const noexcept;
		[[nodiscard]] bool empty() const noexcept;

		[[nodiscard]] bool contains(const Key& key) const;

		void clear();

	private:
		std::unordered_map<Key, Value> m_values;
	};

#pragma region Definitions

	template <typename Key, typename Value>
	template <typename... Args>
	Value& Registry<Key, Value>::emplace(Key key, Args&&... args)
	{
		auto [it, inserted] =
			m_values.try_emplace(
				std::move(key),
				std::forward<Args>(args)...
			);

		return it->second;
		//auto it = m_values.insert_or_assign(std::move(key), Value{ std::forward<Args>(args)... });
		//return it->second;
	}

	template <typename Key, typename Value>
	void Registry<Key, Value>::insert(Key key, Value value)
	{
		m_values.insert_or_assign(std::move(key), std::move(value));
	}

	template <typename Key, typename Value>
	bool Registry<Key, Value>::remove(const Key& key)
	{
		if (m_values.erase(key) > 0)
			return true;

		return false;
	}

	template <typename Key, typename Value>
	const Value& Registry<Key, Value>::get(const Key& key) const
	{
		return m_values.at(key);
	}

	template <typename Key, typename Value>
	Value& Registry<Key, Value>::get(const Key& key)
	{
		return m_values.at(key);
	}

	template <typename Key, typename Value>
	const Value* Registry<Key, Value>::tryGet(const Key& key) const
	{
		if (auto it = m_values.find(key); it != m_values.end())
		{
			return &it->second;
		}

		return nullptr;
	}

	template <typename Key, typename Value>
	Value* Registry<Key, Value>::tryGet(const Key& key)
	{
		if (auto it = m_values.find(key); it != m_values.end())
		{
			return &it->second;
		}

		return nullptr;
	}

	template <typename Key, typename Value>
	size_t Registry<Key, Value>::size() const noexcept
	{
		return m_values.size();
	}

	template <typename Key, typename Value>
	bool Registry<Key, Value>::empty() const noexcept
	{
		return m_values.empty();
	}

	template <typename Key, typename Value>
	bool Registry<Key, Value>::contains(const Key& key) const
	{
		return m_values.contains(key);
	}

	template <typename Key, typename Value>
	void Registry<Key, Value>::clear()
	{
		m_values.clear();
	}

#pragma endregion

//	// or key value? or id?
//	template <typename Value, typename Key = uint32_t>
//	class Registry
//	{
//	public:
//
//		// why entry here???
//		template <typename Entry, typename... Args> // FInd better name than Entry? maybe class should use Entry instead of T?
//		Value& emplace(std::string name, Key key, Args&&... args); // optional name?
//
//		void insert(Value entry); // acept id?
//
//		[[nodiscard]] const Value& get(Key key) const;
//
//		[[nodiscard]] const Value* tryGet(Key key) const;
//
//		[[nodiscard]] const Value& get(const char* name) const;
//
//		[[nodiscard]] const Value* tryGet(const char* name) const;
//
//
//		[[nodiscard]] const Key getKeyFromName(const char* name) const; // KEEP????
//		// getByname and get by id?!
//
//		[[nodiscard]] size_t size() const;
//
//		[[nodiscard]] bool isValid(Key key) const;
//
//		[[nodiscard]] bool isValid(const std::string& name) const;
//
//		void clear();
//
//	private:
//		struct Tag {};
//
//		std::vector<Value> m_entries;
//
//		//std::unordered_map<std::string, Id> m_namesToIds; // or name to inde?
//		std::unordered_map<std::string, std::size_t> m_namesToIndexes; // or name to inde?  !!!! should registry care about name???????????????????????????
//		std::unordered_map<Key, std::size_t> m_keysToIndexes; // use sparse set?
//
//		// mutex?
//	};
//
//#pragma region Definitions
//
//	template <typename Value, typename Key>
//	template <typename Entry, typename... Args>
//	Value& Registry<Value, Key>::emplace(std::string name, Key key, Args&&... args)
//	{
//		auto& entry = m_entries.emplace_back(std::forward<Args>(args)...);
//
//		std::size_t index = m_entries.size() - 1;
//
//		m_keysToIndexes.insert({ key, index });
//		m_namesToIndexes.insert({ name, index });
//
//		return m_entries.back(); // correct?
//	}
//
//	template <typename Value, typename Key>
//	void Registry<Value, Key>::insert(Value entry)
//	{
//		m_entries.push_back(std::move(entry));
//	}
//
//	template <typename Value, typename Key>
//	const Value& Registry<Value, Key>::get(Key key) const
//	{
//		std::size_t index = m_keysToIndexes.at(key);
//		return m_entries.at(index);
//	}
//
//	template <typename Value, typename Key>
//	const Value* Registry<Value, Key>::tryGet(Key key) const
//	{
//		std::size_t index = m_keysToIndexes.at(key);
//		return &m_entries.at(index);
//	}
//
//	template <typename Value, typename Key>
//	const Value& Registry<Value, Key>::get(const char* name) const
//	{
//		auto index = m_namesToIndexes.at(name);
//		return m_entries.at(index);
//	}
//
//	template <typename Value, typename Key>
//	const Value* Registry<Value, Key>::tryGet(const char* name) const
//	{
//		return nullptr;
//	}
//
//	template <typename Value, typename Key>
//	const Key Registry<Value, Key>::getKeyFromName(const char* name) const
//	{
//		//return (Key)m_namesToIndexes.at(name); // Correct?
//		auto index = (Key)m_namesToIndexes.at(name); // Correct?
//
//		for (const auto& [key, i] : m_keysToIndexes)
//		{
//			if (i == index)
//				return key;
//		}
//
//		return -1;
//		//return m_keysToIndexes.find(index)->first; // correct?
//	}
//
//	template <typename Value, typename Key>
//	size_t Registry<Value, Key>::size() const
//	{
//		return 0;
//	}
//
//	template <typename Value, typename Key>
//	bool Registry<Value, Key>::isValid(Key key) const
//	{
//		return false;
//	}
//
//	template <typename Value, typename Key>
//	bool Registry<Value, Key>::isValid(const std::string& name) const
//	{
//		return m_namesToIndexes.contains(name);
//	}
//
//	template <typename Value, typename Key>
//	void Registry<Value, Key>::clear()
//	{
//		m_entries.clear();
//		m_namesToIndexes.clear();
//		m_keysToIndexes.clear();
//	}
//
//#pragma endregion
}