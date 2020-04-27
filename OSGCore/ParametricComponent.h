#pragma once

#include "STLIncluding.h"
#include "BaseObject.h"
#include "OSGCoreExport.h"


class OSGCORE_API ParametricComponent : public BaseObject
{
public:
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
	void SetParam(std::string& strKey, std::string& strValue);
	std::string GetParam(const std::string& strName);
	std::string GetParamResult(const std::string& strName);

protected:
	static bool GetFormulaPairFromString(const std::string& str, std::pair<std::string, std::string>& pair);

protected:
	std::map<std::string, std::string>	m_formulas;
	std::map<std::string, std::string>	m_formulasResult4Cal;
	std::map<std::string, std::string>	m_parentFormulasResult;

	mup::var_maptype	m_formulasResult;

	std::shared_ptr<mup::ParserX>	m_parser;
};