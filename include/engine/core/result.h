#pragma once
#include <optional>
#include <string>

namespace cursed_engine
{
	// Mark methods with constexpr, or not? (small benefit?)

	template <typename T>
	class Result
	{
	public:
		[[nodiscard]] static Result<T> success(T value) noexcept;
		[[nodiscard]] static Result<T> failure(std::string msg) noexcept;

		[[nodiscard]] bool ok() const noexcept;
		[[nodiscard]] const std::string& message() const noexcept;

		[[nodiscard]] const T& peak() const;
		[[nodiscard]] T take();

	private:
		Result(bool success, std::optional<T> value, std::string message);

		bool m_success;
		std::optional<T> m_value;
		std::string m_message;
	};

	// Specialization for void
	template <>
	class Result<void> 
	{
	public:
		[[nodiscard]] static Result success() noexcept;
		[[nodiscard]] static Result failure(std::string msg) noexcept;

		[[nodiscard]] bool ok() const noexcept;
		[[nodiscard]] const std::string& message() const noexcept;

	private:
		Result(bool success, std::string message);

		bool m_success;
		std::string m_message;
	};

#pragma region Definitions

	template <typename T>
	Result<T> Result<T>::success(T value) noexcept
	{
		return Result{ true, std::move(value), std::string{} };
	}

	template <typename T>
	Result<T> Result<T>::failure(std::string msg) noexcept
	{
		return Result{ false, std::nullopt, std::move(msg) };
	}

	template <typename T>
	bool Result<T>::ok() const noexcept
	{
		return m_success;
	}

	template <typename T>
	const std::string& Result<T>::message() const noexcept
	{
		return m_message;
	}

	template <typename T>
	const T& Result<T>::peak() const
	{
		return *m_value;
	}

	template <typename T>
	T Result<T>::take()
	{
		T result = std::move(*m_value);
		m_value.reset();

		return result;
	}

	template <typename T>
	Result<T>::Result(bool success, std::optional<T> value, std::string message)
		: m_success{ success }, m_value{ std::move(value) }, m_message{ std::move(message) }
	{
	}

#pragma endregion
}