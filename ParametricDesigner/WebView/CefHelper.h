#pragma once
#include <3rdPart/cef/include/cef_base.h>
#include "WebApp.h"

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			class CefHelper final
			{
			public:
				CefHelper(const CefHelper&) = delete;
				CefHelper(const CefHelper&&) = delete;
				CefHelper& operator= (const CefHelper&) = delete;
				CefHelper& operator= (const CefHelper&&) = delete;
				~CefHelper();

				static CefHelper& GetGlobal();

				bool Init(HINSTANCE hInstance);
			protected:
				CefHelper();
			private:
				CefRefPtr<WebApp>	m_pWebApp;
				
			};
		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD