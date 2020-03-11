#include "pch.h"

#include "CLROSG.h"
#include <cassert>


CLROSG::Wrapper::Wrapper()
	:ptrInterfaceOSG(new OSGCore::InterfaceOSG())
{
}

CLROSG::Wrapper::~Wrapper()
{
	delete ptrInterfaceOSG;
}

bool CLROSG::Wrapper::Render(IntPtr hwnd)
{
	HWND nativeHWND = (HWND)hwnd.ToPointer();
	if (ptrInterfaceOSG != nullptr)
	{
		return ptrInterfaceOSG->Render(nativeHWND);
	}
	return false;
}

void CLROSG::Wrapper::Destroy()
{
	if (ptrInterfaceOSG != nullptr)
	{
		ptrInterfaceOSG->Destroy();
	}
}

void CLROSG::Wrapper::ShowGrids(bool bEnable)
{
	assert(ptrInterfaceOSG != nullptr);
	//ptrInterfaceOSG->ShowGrids(bEnable);
}

bool CLROSG::Wrapper::IsShowGrids()
{
	assert(ptrInterfaceOSG != nullptr);
	return false;
	//return ptrInterfaceOSG->IsShowGrids();
}

void CLROSG::Wrapper::ShowSelectedObjectCoordinate(bool bEnable)
{
	throw gcnew System::NotImplementedException();
}

bool CLROSG::Wrapper::IsShowSelectedObjectCoordinate()
{
	return false;
}

void CLROSG::Wrapper::ShowDrillingAndGroove(bool bEnable)
{
	throw gcnew System::NotImplementedException();
}

bool CLROSG::Wrapper::IsShowDrillingAndGroove()
{
	return false;
}

void CLROSG::Wrapper::Cale(String^ strVal)
{
	const wchar_t* pChars = (const wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(strVal).ToPointer();
	//std::wstring	wstr = pChars;
	//ptrInterfaceOSG->Cale(wstr);

}
