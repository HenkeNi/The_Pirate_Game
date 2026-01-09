#pragma once
#include "engine/utils/non_copyable.h"
#include "engine/utils/type_traits.h"
#include "engine/utils/type_utils.h"
#include <cassert>

namespace cursed_engine
{
	class Subsystem;

	class SubsystemRegistry : private NonCopyable
	{   
	public:
		// ==================== Construction/Destruction ====================
		SubsystemRegistry() = default;
		~SubsystemRegistry() = default;

		SubsystemRegistry(SubsystemRegistry&&) = delete;
		SubsystemRegistry& operator=(SubsystemRegistry&&) = delete;

		// ==================== Insertion ====================
		template <DerivedFrom<Subsystem> T, typename... Args>
		T& add(Args&&... args);

		// ==================== Access ====================
		template <DerivedFrom<Subsystem> T>
		const T& get() const;

		template <DerivedFrom<Subsystem> T>
		T& get();

		template <DerivedFrom<Subsystem> T>
		const T* tryGet() const;

		template <DerivedFrom<Subsystem> T>
		T* tryGet();

		// ==================== Lookup ====================
		template <DerivedFrom<Subsystem> T>
		bool has() const noexcept;

		// ==================== Iteration ====================
		template <typename Callback>
		void forEach(Callback&& callback) const;

		template <typename Callback>
		void forEach(Callback&& callback);

		// ==================== Queries ====================
		bool isEmpty() const noexcept;

		std::size_t size() const noexcept;

	private:
		using SubsystemPtr = std::unique_ptr<Subsystem>;
		using Subsystems = std::unordered_map<std::type_index, SubsystemPtr>;

		Subsystems m_subsystems;
	};

#pragma region Methods

	template <DerivedFrom<Subsystem> T, typename... Args>
	T& SubsystemRegistry::add(Args&&... args)
	{
		auto [it, inserted] = m_subsystems.try_emplace(
			getTypeIndex<T>(),
			std::make_unique<T>(std::forward<Args>(args)...)
		);

		if (!inserted)
			throw std::runtime_error("Subsystem already added!");

		return *static_cast<T*>(it->second.get());
	}

	template <DerivedFrom<Subsystem> T>
	const T& SubsystemRegistry::get() const
	{
		auto it = m_subsystems.find(getTypeIndex<T>());

		assert(it != m_subsystems.end() && "Subsystem not found in registry!");

		return *static_cast<const T*>(it->second.get());
	}

	template <DerivedFrom<Subsystem> T>
	T& SubsystemRegistry::get()
	{
		return const_cast<T&>(std::as_const(*this).get<T>());
	}

	template <DerivedFrom<Subsystem> T>
	const T* SubsystemRegistry::tryGet() const
	{
		if (auto it = m_subsystems.find(getTypeIndex<T>()); it != m_subsystems.end())
		{
			return static_cast<const T*>(it->second.get());
		}

		return nullptr;
	}

	template <DerivedFrom<Subsystem> T>
	T* SubsystemRegistry::tryGet()
	{
		return const_cast<T*>(std::as_const(*this).tryGet<T>());
	}

	template <DerivedFrom<Subsystem> T>
	bool SubsystemRegistry::has() const noexcept
	{
		return m_subsystems.contains(getTypeIndex<T>());
	}

	template <typename Callback>
	void SubsystemRegistry::forEach(Callback&& callback) const
	{
		for (const auto& [type, subsystem] : m_subsystems)
		{
			if (subsystem) callback(subsystem);
		}
	}

	template <typename Callback>
	void SubsystemRegistry::forEach(Callback&& callback)
	{
		for (const auto& [type, subsystem] : m_subsystems)
		{
			if (subsystem) callback(subsystem);
		}
	}

#pragma endregion
}