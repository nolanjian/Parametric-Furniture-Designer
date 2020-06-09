#pragma once

#include "include/cef_app.h"

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			class WebApp 
				: public CefApp
				//, public CefResourceBundleHandler
				//, public CefBrowserProcessHandler
				//, public CefRenderProcessHandler
			{
			public:
				WebApp();
				virtual ~WebApp();

				virtual void OnBeforeCommandLineProcessing(
					const CefString& process_type,
					CefRefPtr<CefCommandLine> command_line) override;

				virtual void OnRegisterCustomSchemes(
					CefRawPtr<CefSchemeRegistrar> registrar) override;

				//virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override;
				//virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
				//virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

			protected:
			private:
				IMPLEMENT_REFCOUNTING(PFD::GUI::WebView::WebApp);
			};
		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD