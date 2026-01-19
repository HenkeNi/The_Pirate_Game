#pragma once
#include "engine/utils/type_traits.h"
#include "engine/utils/id_generator.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <typeindex>

namespace cursed_engine
{
	// MetaDataBase? MetaStorage? DataStorage?
	// TypeRegistry?
	// Templated or non tempalted?
	// or create a small database => rows and columns... (store data in union or std::any?

	template <typename T, typename ID = uint32_t>
	class TypeRegistry
	{
	public:
		template <typename Entry, typename... Args> // FInd better name than Entry? maybe class should use Entry instead of T?
		T& emplace(std::string name, ID id, Args&&... args);

		void insert(T entry);

		const T& get(const char* name) const;
		
		const T* tryGet(const char* name) const;

		// getByname and get by id?!

		size_t size() const;

		bool isValid(ID id) const;

	private:
		struct Tag {};

		std::vector<T> m_entries;
			
		//std::unordered_map<std::string, ID> m_namesToIDs; // or name to inde?
		std::unordered_map<std::string, ID> m_namesToIndexes; // or name to inde?
		std::unordered_map<ID, int> m_idsToIndexes; // use sparse set?

		// ID Or type index?

		// mutex?
	};

#pragma region Methods

	template <typename T, typename ID>
	template <typename Entry, typename... Args>
	T& TypeRegistry<T, ID>::emplace(std::string name, ID id, Args&&... args)
	{
		auto& entry = m_entries.emplace_back(std::forward<Args>(args)...);

		std::size_t index = m_entries.size() - 1;

		m_idsToIndexes.insert({ id, index });
		m_namesToIndexes.insert({ name, index });
		
		return m_entries.back(); // correct?
	}

	template <typename T, typename ID>
	void TypeRegistry<T, ID>::insert(T entry)
	{
		m_entries.push_back(std::move(entry));
	}

	template <typename T, typename ID>
	const T& TypeRegistry<T, ID>::get(const char* name) const
	{
		return T{};
	}

	template <typename T, typename ID>
	const T* TypeRegistry<T, ID>::tryGet(const char* name) const
	{
		return nullptr;
	}

	template <typename T, typename ID>
	size_t TypeRegistry<T, ID>::size() const
	{
		return 0;
	}

	template <typename T, typename ID>
	bool TypeRegistry<T, ID>::isValid(ID id) const
	{
		return false;
	}

#pragma endregion
}