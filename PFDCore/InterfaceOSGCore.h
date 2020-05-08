#pragma once
#include "OSGCoreExport.h"
#include "STLIncluding.h"
#include <windows.h>


namespace PFDCore
{
	class PFDCORE_API InterfaceOSG
	{
	public:
		InterfaceOSG();
		virtual ~InterfaceOSG();

		bool Render(HWND hwnd);
		void Destroy();

		void SetParam(long long int objectID, const std::wstring& name, const std::wstring& formula);

		void SetOnSelectObjectCallback(void(*ptr)(long long int));

		void LoadScene(const std::string& path);
		void OpenSceneInNewWindow(const std::string& path);

	private:
		void* ptrImp = nullptr;
	};
}