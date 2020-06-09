#include "WXIncluding.h"
#include "App.h"
#include "Frame.h"
#include "IProgramConfig.h"
#include "Utils.h"
#include "spdlog/spdlog.h"

wxIMPLEMENT_APP_NO_MAIN(PFD::GUI::App);

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

			nlohmann::json jsonGUI = PFD::Config::IProgramConfig::GetInstance()->GetJson("GUI");
			std::string utf8AppTitle = jsonGUI["AppTitle"].get<std::string>();

			Frame* pFrame = new Frame(utf8AppTitle, wxDefaultPosition, wxDefaultSize);
			pFrame->Show(true);

			return true;
		}

		int App::OnExit()
		{
			return 0;
		}
	}
}



