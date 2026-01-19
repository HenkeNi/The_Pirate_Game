#pragma once
#include "engine/utils/type_traits.h"
#include "engine/utils/id_generator.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <typeindex>

namespace cursed_engine
{
	// Rename; MetaStorage?
	// Templated or non tempalted?
	// or create a small database => rows and columns... (store data in union or std::any?
	
	template <typename T, typename ID = uint32_t>
	class TypeRegistry
	{
	public:
		template <typename... Ts>
		T& emplace(std::string name, Ts&&... args);

		void insert(T entry);

		const T& get(const char* name) const;
		
		const T* tryGet(const char* name) const;

		// getByname and get by id?!

		size_t size() const;

		bool isValid(TypeId id) const;

	private:
		struct Tag {};

		std::vector<T> m_entries;
			
		std::unordered_map<std::string, ID> m_namesToIDs;
		std::unordered_map<ID, int> m_idsToIndexes; // use sparse set?

		// ID Or type index?

		// mutex?
	};

#pragma region Methods

	template <typename T, typename ID = uint32_t>
	template <typename... Ts>
	T& MetaStorage<T, ID>::emplace(std::string name, Ts&&... args)
	{
		auto id = IDGenerator<Tag>::getID<T>(); // id needed?
	}

	template <typename T, typename ID = uint32_t>
	void MetaStorage<T, ID>::insert(T entry)
	{
		
	}

	const T& get(const char* name) const;

	const T* tryGet(const char* name) const;

	size_t size() const;

	bool isValid(TypeId id) const;


#pragma endregion
}