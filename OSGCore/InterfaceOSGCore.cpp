#include "pch.h"
#include <cassert>
#include "InterfaceOSGCore.h"
#include "ImpOSGCore.h"

static OSGCore::ImpOSGCore* GetImplement(void* ptr)
{
	OSGCore::ImpOSGCore* ptrImp = static_cast<OSGCore::ImpOSGCore*>(ptr);
	assert(ptrImp);
	return ptrImp;
}

OSGCore::InterfaceOSG::InterfaceOSG()
{
	ptrImp = (void*)(new ImpOSGCore());
}

OSGCore::InterfaceOSG::~InterfaceOSG()
{
	delete GetImplement(ptrImp);
}

bool OSGCore::InterfaceOSG::Render(HWND hwnd)
{
	bool bRet = GetImplement(ptrImp)->Render(hwnd);
	return bRet;
}

void OSGCore::InterfaceOSG::Destroy()
{
	GetImplement(ptrImp)->Destory();
}
