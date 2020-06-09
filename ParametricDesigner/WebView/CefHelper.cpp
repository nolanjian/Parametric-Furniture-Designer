#include "CefHelper.h"
#include "include/cef_base.h"
#include <include/cef_app.h>


namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			CefHelper::~CefHelper()
			{
				CefShutdown();
				CefQuitMessageLoop();
			}

			CefHelper& CefHelper::GetGlobal()
			{
				static CefHelper s_CefHelper;
				return s_CefHelper;
			}

			bool CefHelper::Init(HINSTANCE hInstance)
			{
				if (!hInstance)
				{
					return false;
				}

				// Enable High-DPI support on Windows 7 or newer.
				CefEnableHighDPISupport();

				// Parse command-line arguments.
				CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
				command_line->InitFromString(::GetCommandLineW());

				m_pWebApp = new WebApp();
				CefMainArgs args(hInstance);
				int nCode = CefExecuteProcess(args, m_pWebApp, nullptr);
				if (nCode >= 0)
					return false;

				CefSettings settings;
				settings.multi_threaded_message_loop = true;
				settings.no_sandbox = true;

				bool bRet = CefInitialize(args, settings, m_pWebApp, nullptr);
				return bRet;
			}

			CefHelper::CefHelper()
			{
			}

		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD
