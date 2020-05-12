#pragma once

#include "PFDCoreExport.h"
#include "STLIncluding.h"
#include "BaseObject.h"
#include "spdlog/spdlog.h"

namespace PFDCore
{
	class PFDCORE_API ParametricComponent : public BaseObject
	{
	public:
		virtual bool Update();

		virtual bool InitFromParams() { return true; };

		bool UpdateFormulas();

		const mup::var_maptype& FormulasResult();
		const std::map<mup::string_type, mup::string_type>& GetFormulas() { return m_formulas; }

		bool ReInitParser();
		bool SetParentFormulars();

		bool UpdateSelfFormulas();
		bool SetOneLine(const mup::string_type& line);

		bool ParseParams(const nlohmann::json::value_type& params);
		bool ImportParams(const fx::gltf::Node& node);

		static bool regexParseFormular(const mup::string_type& strFormular, mup::string_type& strKey, mup::string_type& strVal);
		static bool regexParseKV(mup::string_type& strKV);

		bool SetParam(const mup::string_type& strFormular);
		bool SetParam(mup::string_type& strKey, mup::string_type& strValue);
		mup::string_type GetParam(const mup::string_type& strName);
		mup::string_type GetParamResult(const mup::string_type& strName);

	protected:
		static bool GetFormulaPairFromString(const mup::string_type& str, std::pair<mup::string_type, mup::string_type>& pair);

	protected:
		// formulas k/v string before cal
		std::map<mup::string_type, mup::string_type>	m_formulas;
		// formulas cale result
		mup::var_maptype	m_formulasResult;
		// parse tool
		std::shared_ptr<mup::ParserX>	m_parser;
	};
}