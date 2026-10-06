#pragma once
#include <limits>
#include <cstdint>

namespace cursed_engine
{
	enum class AudioType : uint8_t
	{
		Sfx,
		Music,
		Ambience,
		Voice,
		UI,
		Count
	};

	struct AudioPlaybackHandle
	{
		using Id = uint32_t;
		using Generation = uint32_t;

		constexpr AudioPlaybackHandle() = default;

		constexpr AudioPlaybackHandle(Id id, Generation generation, AudioType audioType)
			: id{ id }, generation{ generation }, audioType{ audioType }
		{
		}

		constexpr explicit operator bool() const { return id != INVALID_ID; }

		constexpr bool operator==(const AudioPlaybackHandle&) const = default;

		// does static implies inline?
		[[nodiscard]] inline static AudioPlaybackHandle invalid() noexcept { return AudioPlaybackHandle{ INVALID_ID, 0, AudioType::Count }; }

		static constexpr Id INVALID_ID = std::numeric_limits<Id>::max();

		Id id{ INVALID_ID }; // or name index?
		Generation generation{};
		AudioType audioType{};
	};
}