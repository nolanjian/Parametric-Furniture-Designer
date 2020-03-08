#pragma once

#include "../OSGCore/OSGCore.h"

#include <memory>

using namespace System;

namespace CLROSG {
	public ref class Wrapper
	{
	public:
		Wrapper();
		bool Render(IntPtr hwnd);
		void Destroy();
		
		void ShowGrids(bool bEnable);
		bool IsShowGrids();
	private:

		OSGCore::OSGAdapt* ptrOSGAdapt = nullptr;
	};
}
