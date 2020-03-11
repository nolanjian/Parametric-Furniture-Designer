#pragma once

#include "../OSGCore/InterfaceOSGCore.h"

#include <memory>

using namespace System;

namespace CLROSG {
	public ref class Wrapper
	{
	public:
		Wrapper();
		~Wrapper();
		bool Render(IntPtr hwnd);
		void Destroy();

		void ShowGrids(bool bEnable);
		bool IsShowGrids();

		void ShowSelectedObjectCoordinate(bool bEnable);
		bool IsShowSelectedObjectCoordinate();

		void ShowDrillingAndGroove(bool bEnable);
		bool IsShowDrillingAndGroove(); 

		void Cale(String^ strVal);
	private:

		OSGCore::InterfaceOSG* ptrInterfaceOSG = nullptr;
	};
}
