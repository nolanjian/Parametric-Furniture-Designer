#include "pch.h"
#include "Panel.h"

Panel::Panel()
	:ParametricComponent()
{

}

Panel::~Panel()
{
}

bool Panel::InitFromParams()
{
	if (m_formulasResult.find("W") != m_formulasResult.end())
	{
		dWidth = m_formulasResult["W"]->AsIValue()->GetFloat();
	}
	if (m_formulasResult.find("D") != m_formulasResult.end())
	{
		dDepth = m_formulasResult["D"]->AsIValue()->GetFloat();
	}
	if (m_formulasResult.find("H") != m_formulasResult.end())
	{
		dHeight = m_formulasResult["H"]->AsIValue()->GetFloat();
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
