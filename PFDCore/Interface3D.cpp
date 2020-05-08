#include "pch.h"
#include "Interface3D.h"
#include "Implement3D.h"

namespace PFDCore
{

	static Implement3D* GetImplement(void* ptr)
	{
		Implement3D* ptrImp = static_cast<Implement3D*>(ptr);
		assert(ptrImp);
		return ptrImp;
	}

	Interface3D::Interface3D()
	{
		ptrImp = (void*)(new Implement3D());
	}

	Interface3D::~Interface3D()
	{
		delete GetImplement(ptrImp);
	}

	bool Interface3D::Render(HWND hwnd)
	{
		bool bRet = GetImplement(ptrImp)->Render(hwnd);
		return bRet;
	}

	void Interface3D::Destroy()
	{
		GetImplement(ptrImp)->Destory();
	}

	void Interface3D::SetParam(long long int objectID, const std::wstring& name, const std::wstring& formula)
	{
	}

	void Interface3D::SetOnSelectObjectCallback(void(*ptr)(long long int))
	{

	}

	void Interface3D::LoadScene(const std::string& path)
	{
		GetImplement(ptrImp)->LoadScene(path);
	}

	void Interface3D::CloseScene()
	{
		GetImplement(ptrImp)->CloseScene();
	}

	void Interface3D::OpenSceneInNewWindow(const std::string& path)
	{
		GetImplement(ptrImp)->OpenSceneInNewWindow(path);
	}


};