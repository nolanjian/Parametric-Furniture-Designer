#pragma once
#include "ParametricComponent.h"

class Panel : public ParametricComponent
{
public:
	Panel();
	virtual ~Panel();
	virtual bool InitFromParams() override;

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