#pragma once
#include "engine/math/vec2.hpp"
#include "engine/platform/input_types.h"
#include <filesystem>

// TODO; add a SettingsChangedEvent?

namespace cursed_engine
{
	class EventBus;
	
	template <typename T>
	class Result;

	struct AssetSettings
	{
	};
	
	struct AudioSettings
	{
		float masterVolume{};
		float sfxVolume{};
		float musicVolume{};
		float ambienceVolume{};
		float voiceVolume{};
		float uiVolume{};
	};
	
	struct GraphicsSettings
	{
		// brightness...
		bool vsync{};
	};
	
	struct InputSettings
	{
		KeyBindings keyBindings;
		float mouseSensitivity = 1.f;
	};
	
	struct LocalizationSettings
	{
		std::string language;
	};

	struct WindowSettings
	{
		IVec2 size{};
		bool fullscreen;
	};
	
	class Settings
	{
	public:
		Settings(EventBus& eventBus);

		Result<void> save(const std::filesystem::path& path);
		Result<void> load(const std::filesystem::path& path);


		void applySettings(); // Sends out event?????????????????????????????????+ (only downside is that event coudl be missed...)

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

		// does settings provide acess to isMuted, isVsync, etc?
		void setMasterVolume();
		void setMuted(bool shouldMute); // or muteAll? mute/unmute? toggleMute?


		inline const AssetSettings& getAssetSettings() const noexcept { return m_assets; }
		inline const AudioSettings& getAudioSettings() const noexcept { return m_audio; }
		inline const GraphicsSettings& getGraphicsSettings() const noexcept { return m_graphics; }

		inline const InputSettings& getInputSettings() const noexcept { return m_input; }
		inline const LocalizationSettings& getLocalizationSettings() const noexcept { return m_localization; }
		inline const WindowSettings& getWindowSettings() const noexcept { return m_window; }

		// static std::filesystem::path getConfigPath(); // or use this in game???????
		static std::filesystem::path getDefaultPath() noexcept; // fix?

	private:
		AssetSettings m_assets;
		AudioSettings m_audio;
		GraphicsSettings m_graphics;
		InputSettings m_input;
		LocalizationSettings m_localization;
		WindowSettings m_window;

		EventBus& m_eventBus;
	};
}