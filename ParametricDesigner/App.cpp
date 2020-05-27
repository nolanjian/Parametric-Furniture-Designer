#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"
#include "IProgramConfig.h"
#include "Utils.h"
#include "spdlog/spdlog.h"

wxIMPLEMENT_APP(PFD::GUI::App);

namespace PFD
{
	namespace GUI
	{
		App::App()
		{
			logger->info("Program Start");
		}

		App::~App()
		{
			logger->info("Program Exit");
			spdlog::shutdown();
		}

		bool App::OnInit()
		{
			bool bRet = wxApp::OnInit();
			if (!bRet)
				return false;

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



