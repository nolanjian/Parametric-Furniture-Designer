#include "OSGCLR.h"
#include <cassert>


OSGCLR::Wrapper::Wrapper()
	:ptrInterfaceOSG(new OSGCore::InterfaceOSG())
{
}

OSGCLR::Wrapper::~Wrapper()
{
	delete ptrInterfaceOSG;
}

bool OSGCLR::Wrapper::Render(IntPtr hwnd)
{
	HWND nativeHWND = (HWND)hwnd.ToPointer();
	if (ptrInterfaceOSG != nullptr)
	{
		return ptrInterfaceOSG->Render(nativeHWND);
	}
	return false;
}

void OSGCLR::Wrapper::Destroy()
{
	if (ptrInterfaceOSG != nullptr)
	{
		ptrInterfaceOSG->Destroy();
	}
}

void OSGCLR::Wrapper::ShowGrids(bool bEnable)
{
	assert(ptrInterfaceOSG != nullptr);
	//ptrInterfaceOSG->ShowGrids(bEnable);
}

bool OSGCLR::Wrapper::IsShowGrids()
{
	assert(ptrInterfaceOSG != nullptr);
	return false;
	//return ptrInterfaceOSG->IsShowGrids();
}

void OSGCLR::Wrapper::ShowSelectedObjectCoordinate(bool bEnable)
{
	throw gcnew System::NotImplementedException();
}

bool OSGCLR::Wrapper::IsShowSelectedObjectCoordinate()
{
	return false;
}

void OSGCLR::Wrapper::ShowDrillingAndGroove(bool bEnable)
{
	throw gcnew System::NotImplementedException();
}

bool OSGCLR::Wrapper::IsShowDrillingAndGroove()
{
	return false;
}

void OSGCLR::Wrapper::Cale(String^ strVal)
{
	const wchar_t* pChars = (const wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(strVal).ToPointer();
	//std::wstring	wstr = pChars;
	//ptrInterfaceOSG->Cale(wstr);

}

void OSGCLR::Wrapper::SetModelPath(String^ strPath)
{
	assert(ptrInterfaceOSG);
	std::wstring	wstr;
	String2CPPWString(strPath, wstr);
	ptrInterfaceOSG->SetModelPath(wstr);
}

void OSGCLR::Wrapper::CleanParam()
{
}

void OSGCLR::Wrapper::SetParams(Int64 objID, String^ paramJson)
{
}

String^ OSGCLR::Wrapper::GetParams(Int64 objID)
{
	return gcnew String("");
}

void OSGCLR::Wrapper::SetParam(Int64 objID, String^ name, String^ formula)
{
	assert(ptrInterfaceOSG);
	std::wstring	wName;
	String2CPPWString(name, wName);
	std::wstring	wFormula;
	String2CPPWString(formula, wFormula);
	ptrInterfaceOSG->SetParam(objID, wName, wFormula);
}

void OSGCLR::Wrapper::String2CPPWString(String^ from, std::wstring& to)
{
	const wchar_t* pChars = (const wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(from).ToPointer();
	to = pChars;
}
