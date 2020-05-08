#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"
#include "IProgramConfig.h"
#include "Utils.h"
#include "spdlog/spdlog.h"

wxIMPLEMENT_APP(PFDGUI::App);

PFDGUI::App::App()
{
	logger->info("Program Start");
}

PFDGUI::App::~App()
{
	logger->info("Program Exit");
	spdlog::shutdown();
}

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
