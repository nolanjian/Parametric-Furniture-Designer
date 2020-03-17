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
	return false;
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
