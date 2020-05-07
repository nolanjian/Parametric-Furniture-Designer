#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"

wxIMPLEMENT_APP(PFDGUI::App);

bool PFDGUI::App::OnInit()
{
	bool bRet = wxApp::OnInit();
	if (!bRet)
		return false;

	Frame* pFrame = new Frame("Parametric Designer", wxDefaultPosition, wxDefaultSize);
	pFrame->Show(true);

	return true;
}

int PFDGUI::App::OnExit()
{
	return 0;
}

