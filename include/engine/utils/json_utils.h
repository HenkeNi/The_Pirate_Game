#pragma once
//#include "rapidjson/document.h"
#include <memory>
#include <filesystem>
#include <string_view>

namespace cursed_engine
{

	class JsonLoader // or json parer?
	{
	public:
		JsonLoader();
		~JsonLoader();

		bool loadFromFile(const std::filesystem::path& path);

		//JsonLoader& getObject(std::string_view key);

		int getInteger(std::string key, std::string parent = "");

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}