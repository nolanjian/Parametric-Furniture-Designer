#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"
#include "IProgramConfig.h"

wxIMPLEMENT_APP(PFDGUI::App);

bool PFDGUI::App::OnInit()
{
	bool bRet = wxApp::OnInit();
	if (!bRet)
		return false;
	
	Frame* pFrame = new Frame(PFDConfig::IProgramConfig::GetInstance()->GetString("AppTitle"), wxDefaultPosition, wxDefaultSize);
	pFrame->Show(true);

	return true;
}

int PFDGUI::App::OnExit()
{
	return 0;
}

