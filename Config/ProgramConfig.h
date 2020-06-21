#pragma once
#include "IProgramConfig.h"
#include <Commom/STLIncluding.h>
#include <spdlog/spdlog.h>


namespace PFD
{
	namespace Config
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
			std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);
		private:
			nlohmann::json	m_configJson;
		};
	}
}