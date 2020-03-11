#include "pch.h"
#include <cassert>
#include "InterfaceOSGCore.h"
#include "ImplementOSGCore.h"

static OSGCore::ImplementOSGCore* GetImplement(void* ptr)
{
	OSGCore::ImplementOSGCore* ptrImp = static_cast<OSGCore::ImplementOSGCore*>(ptr);
	assert(ptrImp);
	return ptrImp;
}

OSGCore::InterfaceOSG::InterfaceOSG()
{
	ptrImp = (void*)(new ImplementOSGCore());
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
