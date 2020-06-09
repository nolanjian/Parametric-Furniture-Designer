#include "WebApp.h"

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			WebApp::WebApp()
			{

			}

			WebApp::~WebApp()
			{
			}

			void WebApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
			{
			}

			void WebApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
			{
			}

			//CefRefPtr<CefResourceBundleHandler> WebApp::GetResourceBundleHandler()
			//{
			//	return this;
			//}

			//CefRefPtr<CefBrowserProcessHandler> WebApp::GetBrowserProcessHandler()
			//{
			//	return this;
			//}

			//CefRefPtr<CefRenderProcessHandler> WebApp::GetRenderProcessHandler()
			//{
			//	return this;
			//}


		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD
