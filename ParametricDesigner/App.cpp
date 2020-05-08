#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"
#include "IProgramConfig.h"
#include "Utils.h"

wxIMPLEMENT_APP(PFDGUI::App);

bool PFDGUI::App::OnInit()
{
	PFDUtils::InitLogger();

	LOG(INFO) << "app start";
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

