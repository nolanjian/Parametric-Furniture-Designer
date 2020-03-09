#pragma once
#include "BaseObject.h"

class Panel : public BaseObject
{
public:
	void SetDepth(double dVal);
	double GetDepth();
	void SetHeight(double dVal);
	double GetHeight();
	void SetWidth(double dVal);
	double GetWidth();
protected:
private:
	double dDepth = 0.0f;
	double dHeight = 0.0f;
	double dWidth = 0.0f;
};