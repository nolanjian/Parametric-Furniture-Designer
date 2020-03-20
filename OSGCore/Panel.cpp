#include "pch.h"
#include "Panel.h"

Panel::Panel()
	:BaseObject()
{

}

Panel::~Panel()
{
}

bool Panel::InitFromParams()
{
	if (m_formulasResult.find(L"W") != m_formulasResult.end())
	{
		dWidth = m_formulasResult[L"W"]->AsIValue()->GetFloat();
	}
	if (m_formulasResult.find(L"D") != m_formulasResult.end())
	{
		dDepth = m_formulasResult[L"D"]->AsIValue()->GetFloat();
	}
	if (m_formulasResult.find(L"H") != m_formulasResult.end())
	{
		dHeight = m_formulasResult[L"H"]->AsIValue()->GetFloat();
	}

	return true;
}

void Panel::SetDepth(double dVal)
{
	dDepth = dVal;
}

double Panel::GetDepth()
{
	return dDepth;
}

void Panel::SetHeight(double dVal)
{
}

double Panel::GetHeight()
{
	return 0.0;
}

void Panel::SetWidth(double dVal)
{
}

double Panel::GetWidth()
{
	return 0.0;
}
