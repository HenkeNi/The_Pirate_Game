#pragma once

namespace cursed_engine
{
	class ConfigManager;
	class InputHandler;
	class Window;
	
	// Renderer??
	// SettingsChangedEvent?

	// updates engine config?!
	class Settings // listen to eevents?
	{
	public:
		Settings(ConfigManager& config, Window& window);

		// Input


		// Window
		void setResolution();
		
		void enableVSync();
		void disableVSync();
		// toggle vsync...
		[[nodiscard]] bool isVSyncEnabled() const noexcept;

		// Audio
	
		// does settings provide acess to isMuted, isVsync, etc?
		void setMasterVolume();
		void setMuted(bool shouldMute); // or muteAll? mute/unmute? toggleMute?



		bool save();
		// load();


		// Load and applies configs????

	private:
		ConfigManager& m_configManager;
		Window& m_window;
		// store all subsystems or just the ones needed 
		
	};
}