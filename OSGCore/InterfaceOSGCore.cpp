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

void OSGCore::InterfaceOSG::SetModelPath(const std::wstring& path)
{
	GetImplement(ptrImp)->SetModelPath(path);
}

void OSGCore::InterfaceOSG::SetParam(long long int objectID, const std::wstring& name, const std::wstring& formula)
{
}

void OSGCore::InterfaceOSG::SetOnSelectObjectCallback(void(*ptr)(long long int))
{

}
