#pragma once
//#include "ecs_types.h"
#include "engine/utils/type_traits.h"
#include <array>
#include <bitset>
#include <ranges>
#include <cstddef>
#include <algorithm>

namespace cursed_engine
{

	/*
	If Id is a type like a UUID (a 128-bit or 64-bit struct, or some complex type), then:

You can’t safely convert it to size_t to use as an index.

static_cast<std::size_t>(id) will fail to compile or produce nonsense.
	*/

	template <UnsignedIntegral Id, std::size_t signatureSize, std::size_t capacity>
	class SignatureRegistry
	{
	public: 
		using Signature = std::bitset<signatureSize>;
	
		[[nodiscard]] const Signature& getSignature(Id id) const noexcept;

		void setSignature(Id id, const Signature& signature) noexcept;

		void reset(Id id) noexcept;

		void resetAll() noexcept;

		[[nodiscard]] bool isValid(Id id) const noexcept;

	private:
		std::array<Signature, capacity> m_signatures;
	};

#pragma region Methods

	template <UnsignedIntegral Id, std::size_t signatureSize, std::size_t capacity>
	const SignatureRegistry<Id, signatureSize, capacity>::Signature& SignatureRegistry<Id, signatureSize, capacity>::getSignature(Id id) const noexcept
	{
		assert(isValid(id) && "Invalid ID passed to getSignature");
		return m_signatures[id];
	}

	template <UnsignedIntegral Id, std::size_t signatureSize, std::size_t capacity>
	void SignatureRegistry<Id, signatureSize, capacity>::setSignature(Id id, const Signature& signature) noexcept
	{
		assert(isValid(id) && "Invalid ID passed to getSignature");
		m_signatures[id] = signature;
	}

	template <UnsignedIntegral Id, std::size_t signatureSize, std::size_t capacity>
	void SignatureRegistry<Id, signatureSize, capacity>::reset(Id id) noexcept
	{
		assert(isValid(id) && "Invalid ID passed to getSignature");
		m_signatures[id].reset();
	}

	template <UnsignedIntegral Id, std::size_t signatureSize, std::size_t capacity>
	void SignatureRegistry<Id, signatureSize, capacity>::resetAll() noexcept
	{
		std::ranges::for_each(m_signatures, [](Signature& sig) { sig.reset(); });
	}

	template <UnsignedIntegral Id, std::size_t signatureSize, std::size_t capacity>
	bool SignatureRegistry<Id, signatureSize, capacity>::isValid(Id id) const noexcept
	{
		return static_cast<std::size_t>(id) < capacity;
	}

#pragma endregion
}