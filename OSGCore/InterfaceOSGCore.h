#pragma once
#include "OSGCoreExport.h"
#include <windows.h>

namespace OSGCore
{
	class OSGCORE_API InterfaceOSG
	{
	public:
		InterfaceOSG();
		virtual ~InterfaceOSG();

		bool Render(HWND hwnd);
		void Destroy();
	private:
		void* ptrImp = nullptr;
	};
}