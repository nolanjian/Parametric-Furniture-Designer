#pragma once

#include "../OSGCore/OSGCore.h"

#include <memory>

using namespace System;

namespace CLROSG {
	public ref class Wrapper
	{
	public:
		Wrapper();
		void Render(IntPtr hwnd);
		void Destroy();
	private:

		OSGCore::OSGAdapt* ptrOSGAdapt = nullptr;
	};
}
