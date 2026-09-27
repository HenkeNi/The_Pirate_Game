#pragma once
#include "engine/core/settings/engine_config.h"
#include <filesystem>

namespace cursed_engine
{
	class EventBus;
	
	template <typename T>
	class Result;
	
	// SettingsChangedEvent?

	// updates engine config?!
	class Settings // listen to eevents?
	{
	public:
		Settings(EventBus& eventBus); // used for what?

		Result<void> loadConfig(const std::filesystem::path& path);
		void saveConfig(const std::filesystem::path& path);

		void applySettings();

		//[[nodiscard]] bool hasPendingChanges() const noexcept;

		// Input


		// Window
		void setResolution();
		// getResolution
	//	settings provides and sets resolution ?

		// get window resolution
		
		void enableVSync();
		void disableVSync();
		// toggle vsync...
		
		[[nodiscard]] bool isVSyncEnabled() const noexcept;

		// Load and applies configs????

		// Audio
	
		// does settings provide acess to isMuted, isVsync, etc?
		void setMasterVolume();
		void setMuted(bool shouldMute); // or muteAll? mute/unmute? toggleMute?



		bool save();
		// load();

		// temp!! use IVEc??
		inline FVec2 getWindowSize() const { return FVec2{ 1280, 720 }; }



		[[nodiscard]] inline const EngineConfig& getEngineConfig() const noexcept { return m_config; }

		static std::filesystem::path getConfigPath();

	private:
		EngineConfig m_config;
		EventBus& m_eventBus;
	};
}