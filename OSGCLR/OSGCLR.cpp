#include "OSGCLR.h"
#include <cassert>


OSGCLR::Wrapper::Wrapper()
	:ptrInterfaceOSG(new OSGCore::InterfaceOSG())
{
}

OSGCLR::Wrapper::~Wrapper()
{
	delete ptrInterfaceOSG;
}

bool OSGCLR::Wrapper::Render(IntPtr hwnd)
{
	HWND nativeHWND = (HWND)hwnd.ToPointer();
	if (ptrInterfaceOSG != nullptr)
	{
		return ptrInterfaceOSG->Render(nativeHWND);
	}
	return false;
}

void OSGCLR::Wrapper::Destroy()
{
	if (ptrInterfaceOSG != nullptr)
	{
		ptrInterfaceOSG->Destroy();
	}
}

void OSGCLR::Wrapper::ShowGrids(bool bEnable)
{
	assert(ptrInterfaceOSG != nullptr);
	//ptrInterfaceOSG->ShowGrids(bEnable);
}

bool OSGCLR::Wrapper::IsShowGrids()
{
	assert(ptrInterfaceOSG != nullptr);
	return false;
	//return ptrInterfaceOSG->IsShowGrids();
}

void OSGCLR::Wrapper::ShowSelectedObjectCoordinate(bool bEnable)
{
	throw gcnew System::NotImplementedException();
}

bool OSGCLR::Wrapper::IsShowSelectedObjectCoordinate()
{
	return false;
}

void OSGCLR::Wrapper::ShowDrillingAndGroove(bool bEnable)
{
	throw gcnew System::NotImplementedException();
}

bool OSGCLR::Wrapper::IsShowDrillingAndGroove()
{
	return false;
}

void OSGCLR::Wrapper::Cale(String^ strVal)
{
	const wchar_t* pChars = (const wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(strVal).ToPointer();
	//std::wstring	wstr = pChars;
	//ptrInterfaceOSG->Cale(wstr);

}
