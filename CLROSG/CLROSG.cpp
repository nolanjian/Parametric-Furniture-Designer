#include "pch.h"

#include "CLROSG.h"
#include <cassert>


CLROSG::Wrapper::Wrapper()
	:ptrOSGAdapt(new OSGCore::OSGAdapt())
{
}

bool CLROSG::Wrapper::Render(IntPtr hwnd)
{
	HWND nativeHWND = (HWND)hwnd.ToPointer();
	if (ptrOSGAdapt != nullptr)
	{
		return ptrOSGAdapt->Render(nativeHWND);
	}
	return false;
}

void CLROSG::Wrapper::Destroy()
{
	if (ptrOSGAdapt != nullptr)
	{
		ptrOSGAdapt->Destroy();
	}
}

void CLROSG::Wrapper::ShowGrids(bool bEnable)
{
	assert(ptrOSGAdapt != nullptr);
	ptrOSGAdapt->ShowGrids(bEnable);
}

bool CLROSG::Wrapper::IsShowGrids()
{
	assert(ptrOSGAdapt != nullptr);
	return ptrOSGAdapt->IsShowGrids();
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
