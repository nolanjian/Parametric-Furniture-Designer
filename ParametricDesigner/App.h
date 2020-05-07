#pragma once

#include "WXIncluding.h"

namespace PFDGUI
{
	class App : public wxApp
	{
	public:
		virtual bool OnInit() override;
		virtual int OnExit() override;
	};
};