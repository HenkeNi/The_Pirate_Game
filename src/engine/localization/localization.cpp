#include "engine/localization/localization.h"
#include "engine/core/logger.h"
#include "engine/utils/json/json_document.h"
#include "engine/utils/json/json_value.h"

namespace cursed_engine
{
	void Localization::registerLanguage(const std::string& language, const std::filesystem::path& path)
	{
		m_languageToPath.insert({ language, path });
	}

	bool Localization::setLanguage(const std::string& language)
	{
		auto it = m_languageToPath.find(language);
		if (it == m_languageToPath.end())
		{
			Logger::logError("Failed to load " + language + ": unrecognized language!");
			return false;
		}

		bool success = loadLanguage(it->second);

		if (success)
			m_currentLanguage = language;

		return success;
	}

	bool Localization::isCurrentLanguage(const std::string& language) const
	{
		return m_currentLanguage == language;
	}

	const std::string& Localization::getText(const std::string& key) const
	{
		return m_translations.at(key);
	}

	bool Localization::hasText(const std::string& key) const
	{
		return m_translations.contains(key);
	}

	bool Localization::loadLanguage(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			Logger::logError(std::format("Failed to load language - invalid path: {}", path.string()));
			return false;
		}

		JsonDocument jsonDocument;
		const JsonResult result = jsonDocument.loadFromFile(path);

		if (!result.success)
		{
			Logger::logError(std::format("Failed to load language: {}", result.errorMessage));
			return false;
		}

		m_translations.clear(); // or create empty map and then swap?
		
		for (const auto& textValue : jsonDocument["texts"].asArray())
		{
			textValue.forEachProperty([&](const char* id, JsonValue value)
				{
					assert(value.isString() && "Text is not string!");
					assert(!m_translations.contains(id) && "Text is already registered");

					m_translations.insert({ id, value.asString() });
				});

		}

		onLanguageChanged();
	}

	void Localization::onLanguageChanged()
	{
		// TODO; notify language changed!
	}

	void Localization::clear() noexcept
	{
		m_translations.clear();
	}
}