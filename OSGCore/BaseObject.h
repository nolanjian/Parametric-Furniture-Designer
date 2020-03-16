#pragma once
#include <osg/Group>
#include <string>
#include <memory>
#include "../Params/Params.h"
#include "../muparserx/mpParser.h"
#include "../muparserx/mpDefines.h"
#include "../muparserx/mpTypes.h"

class BaseObject : public osg::Group
{
public:
	BaseObject();
	virtual ~BaseObject();

	void SetParent(osg::ref_ptr<BaseObject> parent) { m_parent = parent; }
	osg::ref_ptr<BaseObject> GetParent() { return m_parent; }

	const std::map<std::wstring, std::wstring>& GetFormulas() { return m_formulas; }
	
	bool ReInitParser();
	bool SetParentFormulars();
	bool UpdateFormulas();
	bool UpdateSelfFormulas();
	bool SetOneLine(const std::wstring& line);

	const mup::var_maptype& FormulasResult();

protected:
	std::map<std::wstring, std::wstring>	m_formulas;
	std::map<std::wstring, std::wstring>	m_formulasResult4Cal;
	std::map<std::wstring, std::wstring>	m_parentFormulasResult;

	mup::var_maptype	m_formulasResult;

private:
	bool m_bDirty = true;

	osg::ref_ptr<BaseObject>	m_parent;
	std::shared_ptr<mup::ParserX>	m_parser;
};