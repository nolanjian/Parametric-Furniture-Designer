#include "WXApp/WXIncluding.h"
#include "Utils.h"
#include "WXApp/App.h"
#include "WebView/CefHelper.h"

class LifeHelper
{
public:
	LifeHelper()
	{
		logger->info("PFD APP START");
	}
	~LifeHelper()
	{
		logger->info("PFD APP STOP");
		spdlog::shutdown();
	}
private:
	std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);
};

LifeHelper lifeHelper;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);
	wxDISABLE_DEBUG_SUPPORT();

	PFD::GUI::WebView::CefHelper& objCefHelper = PFD::GUI::WebView::CefHelper::GetGlobal();
	bool bInitCEF = objCefHelper.Init(hInstance);
	if (!bInitCEF)
	{
		logger->error("Init CEF Fail");
		return -1;
	}

	int nWXRet = wxEntry(hInstance, hPrevInstance, NULL, nShowCmd);
	return nWXRet;
}