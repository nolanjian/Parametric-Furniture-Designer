#pragma once
#include <osg/Group>
#include <string>
#include "../Params/Params.h"
#include "../muparserx/mpParser.h"
#include "../muparserx/mpDefines.h"

class BaseObject : public osg::Group
{
public:
	BaseObject();
	virtual ~BaseObject();

	void SetParent(osg::ref_ptr<BaseObject> parent) { m_parent = parent; }
	osg::ref_ptr<BaseObject> GetParent() { return m_parent; }

	const std::map<std::wstring, std::wstring>& GetFormulas() { return m_formulas; }
	const std::map<std::wstring, std::wstring>& GetFormulasResult() { return m_formulas; }
	
	bool SetParentFormulars();
	void UpdateFormulas();



protected:
	std::map<std::wstring, std::wstring>	m_formulas;
	std::map<std::wstring, std::wstring>	m_formulasResult;
	std::map<std::wstring, std::wstring>	m_formulasResult4Cal;
	std::map<std::wstring, std::wstring>	m_parentFormulasResult;

private:
	bool m_bDirty = true;

	osg::ref_ptr<BaseObject>	m_parent;
	mup::ParserX	m_parser = mup::ParserX(mup::EPackages::pckALL_NON_COMPLEX);
};