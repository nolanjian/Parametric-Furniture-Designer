#pragma once
#include "IProgramConfig.h"
#include <map>
#include <string>


namespace PFDConfig
{
	class ProgramConfig : public IProgramConfig
	{
	public:
		ProgramConfig();
		virtual ~ProgramConfig();
		virtual bool ReLoad() override;
		virtual std::string GetString(const std::string& strKey) override;
		virtual long long int GetInt(const std::string& strKey) override;
		virtual double GetDouble(const std::string& strKey) override;
		virtual nlohmann::json GetJson(const std::string& strKey) override;
	protected:

	private:
		nlohmann::json	m_configJson;
	};
}