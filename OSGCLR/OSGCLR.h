#pragma once

#include "../OSGCore/InterfaceOSGCore.h"

#include <memory>

using namespace System;



namespace OSGCLR {
	public delegate void OnSelectObjectDelegate(Int64 objID);

	public ref class Wrapper
	{
	public:

		Wrapper();
		~Wrapper();
		bool Render(IntPtr hwnd);
		void Destroy();

		void ShowGrids(bool bEnable);
		bool IsShowGrids();

		void ShowSelectedObjectCoordinate(bool bEnable);
		bool IsShowSelectedObjectCoordinate();

		void ShowDrillingAndGroove(bool bEnable);
		bool IsShowDrillingAndGroove();

		void Cale(String^ strVal);

		void SetModelPath(String^ strPath);

		void CleanParam();
		void SetParams(Int64 objID, String^ paramJson);
		String^ GetParams(Int64 objID);
		void SetParam(Int64 objID, String^ name, String^ formula);

		void SetHandlerOnSelectObject(OnSelectObjectDelegate^ handler);
		void OnSelectObject(Int64 objID);

	protected:
		void String2CPPWString(String^ from, std::wstring& to);

		OnSelectObjectDelegate^ handlerOnSelectObject = nullptr;
	private:

		PFDCore::InterfaceOSG* ptrInterfaceOSG = nullptr;
	};
}