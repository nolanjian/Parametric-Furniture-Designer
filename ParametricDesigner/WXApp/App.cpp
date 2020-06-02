#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"
#include "IProgramConfig.h"
#include "Utils.h"
#include "spdlog/spdlog.h"

#if PFD_USE_WX_MAIN
wxIMPLEMENT_APP(PFD::GUI::App);
#else
wxIMPLEMENT_APP_NO_MAIN(PFD::GUI::App);
#endif // PFD_USE_WX_MAIN

namespace PFD
{
	namespace GUI
	{
		App::App()
		{
		}

		App::~App()
		{
		}

		bool App::OnInit()
		{
			bool bRet = wxApp::OnInit();
			if (!bRet)
			{
				logger->info("App Init Fail");
				return false;
			}

			Frame* pFrame = new Frame(PFD::Config::IProgramConfig::GetInstance()->GetString("AppTitle"), wxDefaultPosition, wxDefaultSize);
			pFrame->Show(true);

			return true;
		}

		int App::OnExit()
		{
			return 0;
		}
	}
}



