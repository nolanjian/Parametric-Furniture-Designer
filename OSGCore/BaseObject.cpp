#include "pch.h"
#include "BaseObject.h"

BaseObject::BaseObject()
	:osg::Group()
{
}

BaseObject::~BaseObject()
{
}

bool BaseObject::SetParentFormulars()
{
	osg::ref_ptr<BaseObject> pParent = GetParent();
	if (!pParent.valid())
	{
		return false;
	}

	try
	{
		osg::ref_ptr<BaseObject> pParent = GetParent();
		m_parser.ClearVar();
		assert(m_parser.GetExprVar().size());
		for (auto pKV : pParent->GetFormulasResult())
		{
			mup::string_type	formular = _T("Parent_") + pKV.first + _T("=") + pKV.second;
			m_parser.SetExpr(formular);
		}
		assert(pParent->GetFormulasResult().size() == m_parser.GetExprVar().size());

		return true;
	}
	catch (const mup::ParserError & e)
	{
		// Parse Parent Params Error
		return false;
	}
	catch (const std::exception & e)
	{
		return false;
	}
}

void BaseObject::UpdateFormulas()
{
	// cale this
	osg::ref_ptr<BaseObject> pParent = GetParent();

	m_parentFormulasResult.clear();
	for (auto pKV : pParent->GetFormulasResult())
	{
		m_parentFormulasResult[L"Parent_" + pKV.first] = pKV.second;
	}

	m_formulasResult4Cal.clear();
	for (auto pKV : GetFormulas())
	{
		static std::wstring wsParent(L"Parent.");
		static std::wstring wsrParent(L"Parent_");
		std::wstring val = pKV.second;

		size_t	pos = val.find(wsParent);
		while (pos != std::wstring::npos)
		{
			val = val.replace(pos, wsParent.length(), wsrParent);
			pos = val.find(wsParent);
		}

		m_formulasResult4Cal[pKV.first] = val;
	}

	// begin to cal
	try
	{
		mup::ParserX	parser(mup::pckALL_NON_COMPLEX);
		osg::ref_ptr<BaseObject> pParent = GetParent();
		assert(parser.GetExprVar().size());
		for (auto pKV : pParent->GetFormulasResult())
		{
			mup::string_type	formular = _T("Parent_") + pKV.first + _T("=") + pKV.second;
			parser.SetExpr(formular);
		}
		assert(pParent->GetFormulasResult().size() == parser.GetExprVar().size());


	}
	catch (const mup::ParserError & e)
	{

	}
	catch (const std::exception& e)
	{

	}
	
	for (auto& child : _children)
	{
		BaseObject* pBaseObject = dynamic_cast<BaseObject*>(child.get());
		if (pBaseObject)
		{
			pBaseObject->UpdateFormulas();
		}
	}
}
