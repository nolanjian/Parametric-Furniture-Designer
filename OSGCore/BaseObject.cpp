#include "pch.h"
#include "BaseObject.h"

BaseObject::BaseObject()
	:Group()
{
}

BaseObject::~BaseObject()
{
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

	}
	catch (const std::exception&)
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
