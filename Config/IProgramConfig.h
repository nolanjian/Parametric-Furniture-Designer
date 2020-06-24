#pragma once
#include <Config/Export.h>
#include <Utils/Utils.h>
#include <nlohmann/json.hpp>

namespace PFD
{
	namespace Config
	{
		class PFD_CONFIG_API IProgramConfig
		{
		public:
			static IProgramConfig* GetInstance();
			virtual ~IProgramConfig() {};
			virtual bool ReLoad() = 0;
			virtual std::string GetString(const std::string& strKey) = 0;
			virtual long long int GetInt(const std::string& strKey) = 0;
			virtual double GetDouble(const std::string& strKey) = 0;
			virtual nlohmann::json GetJson(const std::string& strKey) = 0;
		protected:

		private:
		};
	}
}