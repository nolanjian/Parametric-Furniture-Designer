#pragma once
#include "PFDCoreExport.h"
#include "STLIncluding.h"
#include <windows.h>


namespace PFDCore
{
	class PFDCORE_API Interface3D
	{
	public:
		Interface3D();
		virtual ~Interface3D();

		bool Render(HWND hwnd);
		void Destroy();

		void SetParam(long long int objectID, const std::wstring& name, const std::wstring& formula);

		void SetOnSelectObjectCallback(void(*ptr)(long long int));

		void LoadScene(const std::string& path);
		void CloseScene();
		void OpenSceneInNewWindow(const std::string& path);

	private:
		void* ptrImp = nullptr;
	};
}