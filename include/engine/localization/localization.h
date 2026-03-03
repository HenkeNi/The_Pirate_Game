#pragma once
#include "engine/core/subsystem.h"
#include <filesystem>
#include <string>
#include <unordered_map>

// Store language in JSON document? OR just as key?

namespace cursed_engine
{
	class Localization : public Subsystem
	{
	public:
		void registerLanguage(const std::string& language, const std::filesystem::path& path);
		bool setLanguage(const std::string& language); // send event that notifies text components that they are dirty!

		[[nodiscard]] inline const std::string& getCurrentLanguage() const { return m_currentLanguage; }
		[[nodiscard]] bool isCurrentLanguage(const std::string& language) const;

		[[nodiscard]] const std::string& getText(const std::string& key) const;
		[[nodiscard]] bool hasText(const std::string& key) const;
		// std.:string format(key, args...);

		// overload [] operator?

	private:
		bool loadLanguage(const std::filesystem::path& path);
		void onLanguageChanged();
		void clear() noexcept;
		
		std::unordered_map<std::string, std::filesystem::path> m_languageToPath;
		
		std::unordered_map<std::string, std::string> m_translations; // better name! stirngs?
		std::string m_currentLanguage;

		// store id (stirngg) to text (read from json)...
	};
}