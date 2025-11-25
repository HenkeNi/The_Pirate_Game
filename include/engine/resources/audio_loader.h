#pragma once
#include <filesystem>

namespace cursed_engine
{
	class Audio;
	//class AudioController;

	class AudioLoader
	{
	public:
		[[nodiscard]] std::unique_ptr<Audio> load(const std::filesystem::path& path) const;
		//[[nodiscard]] std::unique_ptr<Audio> load(AudioController& controller, const std::filesystem::path& path) const;
	};
}