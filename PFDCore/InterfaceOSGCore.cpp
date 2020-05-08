#include "pch.h"
#include "InterfaceOSGCore.h"
#include "ImplementOSGCore.h"

static PFDCore::ImplementOSGCore* GetImplement(void* ptr)
{
	PFDCore::ImplementOSGCore* ptrImp = static_cast<PFDCore::ImplementOSGCore*>(ptr);
	assert(ptrImp);
	return ptrImp;
}

PFDCore::InterfaceOSG::InterfaceOSG()
{
	ptrImp = (void*)(new ImplementOSGCore());
}

PFDCore::InterfaceOSG::~InterfaceOSG()
{
	delete GetImplement(ptrImp);
}

bool PFDCore::InterfaceOSG::Render(HWND hwnd)
{
	bool bRet = GetImplement(ptrImp)->Render(hwnd);
	return bRet;
}

void PFDCore::InterfaceOSG::Destroy()
{
	GetImplement(ptrImp)->Destory();
}

void PFDCore::InterfaceOSG::SetParam(long long int objectID, const std::wstring& name, const std::wstring& formula)
{
}

void PFDCore::InterfaceOSG::SetOnSelectObjectCallback(void(*ptr)(long long int))
{

}

void PFDCore::InterfaceOSG::LoadScene(const std::string& path)
{
	GetImplement(ptrImp)->LoadScene(path);
}

void PFDCore::InterfaceOSG::OpenSceneInNewWindow(const std::string& path)
{
	GetImplement(ptrImp)->OpenSceneInNewWindow(path);
}
