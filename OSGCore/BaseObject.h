#pragma once
#include <osg/Group>
#include <string>

class BaseObject : public osg::Group
{
public:
	BaseObject();
	virtual ~BaseObject();

	void SetParent(osg::ref_ptr<BaseObject> parent) { m_parent = parent; }
	osg::ref_ptr<BaseObject> GetParent() { return m_parent; }

	const std::map<std::wstring, std::wstring>& GetFormulas() { return m_formulas; }
	void SetFormulas(const std::map<std::wstring, std::wstring>& formulas) { m_formulas = formulas; }
	void SetFormula(const std::wstring& key, const std::wstring& formula) { m_formulas[key] = formula; }
	const std::map<std::wstring, std::wstring>& GetFormulasResult() { return m_formulasResult; }

	void UpdateFormulas();

protected:
	std::map<std::wstring, std::wstring>	m_formulas;
	std::map<std::wstring, std::wstring>	m_formulasResult;
	std::map<std::wstring, std::wstring>	m_formulasResult4Cal;
	std::map<std::wstring, std::wstring>	m_parentFormulasResult;
private:
	osg::ref_ptr<BaseObject>	m_parent;
	
};