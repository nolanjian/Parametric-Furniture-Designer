#include "pch.h"

#include "CLROSG.h"


CLROSG::Wrapper::Wrapper()
	:ptrOSGAdapt(new OSGCore::OSGAdapt())
{
}

void CLROSG::Wrapper::Render(IntPtr hwnd)
{
	HWND nativeHWND = (HWND)hwnd.ToPointer();
	if (ptrOSGAdapt != nullptr)
	{
		ptrOSGAdapt->Render(nativeHWND);
	}
}

void CLROSG::Wrapper::Destroy()
{
	if (ptrOSGAdapt != nullptr)
	{
		ptrOSGAdapt->Destroy();
	}
}
