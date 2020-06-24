/*****************************************************************//**
 * \file   Parametric.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/24
 * 
 *********************************************************************/
#pragma once
#include <Component/Export.h>
#include <Component/BaseObject.h>
#include <Utils/Utils.h>
#include <spdlog/spdlog.h>

#include <muparserx/mpParser.h>
#include <muparserx/mpDefines.h>
#include <muparserx/mpTypes.h>

namespace PFD
{
	namespace Component
	{
		class PFD_COMPONENT_API Parametric : public BaseObject
		{
		public:
			static std::shared_ptr<spdlog::logger> logger;

			virtual bool Update();

			virtual bool InitFromParams() { return true; };

			bool UpdateFormulas();

			const mup::var_maptype& FormulasResult();
			const std::map<std::string, std::string>& GetFormulas() { return m_formulas; }

			bool ReInitParser();
			bool SetParentFormulars();

			bool UpdateSelfFormulas();
			bool SetOneLine(const std::string& line);

			bool ParseParams(const nlohmann::json::value_type& params);
			bool ImportParams(const fx::gltf::Node& node);

			static bool regexParseFormular(const std::string& strFormular, std::string& strKey, std::string& strVal);
			static bool regexParseKV(std::string& strKV);

			bool SetParam(const std::string& strFormular);
			bool SetParam(std::string& strKey, std::string& strValue);
			std::string GetParam(const std::string& strName);
			std::string GetParamResult(const std::string& strName);

		protected:
			static bool GetFormulaPairFromString(const std::string& str, std::pair<std::string, std::string>& pair);

		protected:
			// formulas k/v string before cal
			std::map<std::string, std::string>	m_formulas;
			// formulas cale result
			mup::var_maptype	m_formulasResult;
			// parse tool
			std::shared_ptr<mup::ParserX>	m_parser;
		};
	}
}