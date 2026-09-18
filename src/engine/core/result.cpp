#include "engine/core/result.h"

namespace cursed_engine
{
	Result<void> Result<void>::failure(std::string msg) noexcept
	{
		return Result<void>{ false, std::move(msg) };
	}

	Result<void> Result<void>::Result::success() noexcept
	{
		return Result<void>{ true, std::string{} };
	}

	bool Result<void>::ok() const noexcept
	{
		return m_success;
	}

	const std::string& Result<void>::message() const noexcept
	{
		return m_message;
	}

	Result<void>::Result(bool success, std::string message)
		: m_success{ success }, m_message{ std::move(message) }
	{
	}
}