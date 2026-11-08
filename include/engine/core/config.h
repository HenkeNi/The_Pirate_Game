#pragma once
#include "engine/math/vec2.hpp"
#include <filesystem>

// Make load function static? constructor private?
// TODO: assert if not initialized?

namespace cursed_engine
{
	template <typename T>
	class Result;

	struct Config
	{
		Result<void> load(const std::filesystem::path& path);

		static std::filesystem::path getDefaultPath() noexcept;

		struct AppInfo
		{
			std::string name = "";
			std::string identifier = "";
			std::string version = "";
		} appInfo;

		struct Audio
		{
			int sfxTrackCount{};
			int musicTrackCount{};
			int ambienceTrackCount{};
			int voiceTrackCount{};
			int uiTrackCount{};
		} audio;

		struct Network
		{
			int port;
			std::string ip;
		} network;

		struct Resource
		{
			/*std::vector<std::string> validTextureFormats;
			std::vector<std::string> validAudioFormats;
			std::vector<std::string> validFontFormats;*/

			//std::unordered_map<std::string, std::string> resourceIdToPath; // or create ResourceRegistry class? same for all types??
			//bool lazyLoad = true;

			//std::filesystem::path assetRoot;
			uint32_t framesBeforeUnload = 4000; // need both?
			float offloadDelaySeconds = 5.0f;
			bool autoOffload = true;

		} resource;

		struct Window
		{
			std::string iconPath = "";
			// Position
			int xPos = 0;
			int yPos = 0;
			IVec2 minSize{};

			bool alwaysOnTop = true;
			bool borderless = true;
		} window;
	};
}