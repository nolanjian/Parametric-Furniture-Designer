#include "ProgramConfig.h"
#include <filesystem>
#include <fstream>
#include <iostream>


namespace PFDConfig
{
	ProgramConfig::ProgramConfig()
	{
		ReLoad();
	}
	ProgramConfig::~ProgramConfig()
	{
	}
	bool ProgramConfig::ReLoad()
	{
		try
		{
			m_configJson.clear();
			std::filesystem::path	configPath = std::filesystem::current_path().string() + ("/Program.json");
			std::ifstream ifs(configPath);
			auto b = ifs.is_open();

			ifs >> m_configJson;

			return true;
		}
		catch (const nlohmann::detail::parse_error& ex)
		{
			logger->error(ex.what());
		}
		catch (const nlohmann::detail::exception& ex)
		{
			logger->error(ex.what());
		}
		catch (const std::filesystem::filesystem_error& ex)
		{
			logger->error(ex.what());
		}
		catch (const std::exception& ex)
		{
			logger->error(ex.what());
		}

		return false;
	}
	std::string ProgramConfig::GetString(const std::string& strKey)
	{
		if (m_configJson.contains(strKey))
		{
			auto obj = m_configJson[strKey];
			if (obj.is_string())
			{
				return obj.get<std::string>();
			}
		}
		return "";
	}
	long long int ProgramConfig::GetInt(const std::string& strKey)
	{
		if (m_configJson.contains(strKey))
		{
			auto obj = m_configJson[strKey];
			if (obj.is_number())
			{
				return obj.get<long long int>();
			}
		}
		return 0;
	}
	double ProgramConfig::GetDouble(const std::string& strKey)
	{
		if (m_configJson.contains(strKey))
		{
			auto obj = m_configJson[strKey];
			if (obj.is_number_float())
			{
				return obj.get<double>();
			}
		}
		return 0.0;
	}
	nlohmann::json ProgramConfig::GetJson(const std::string& strKey)
	{
		if (m_configJson.contains(strKey))
		{
			return m_configJson[strKey];
		}
		return nlohmann::json();
	}
};


