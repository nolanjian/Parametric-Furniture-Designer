#pragma once
#include "../WXApp/WXIncluding.h"
#include "ClientHandler.h"
#include <string>

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			class ClientHandler;

			class Widget : public wxControl
			{
			public:
				explicit Widget();
				explicit Widget(wxWindow* parent
					, const std::string& utf8URL
					, wxWindowID id = wxID_ANY
					, const wxPoint& pos = wxDefaultPosition
					, const wxSize& size = wxDefaultSize
					, long style = 0
					, const wxValidator& validator = wxDefaultValidator
					, const wxString& name = wxControlNameStr);
				virtual ~Widget();

				bool LoadURL(const std::string& utf8URL);
				void CloseCefBrowser();
				void RunScript(const std::string& utf8Script);
			protected:
				void OnSize(wxSizeEvent& event);
			private:
				void InitCEF();
				void InitEvents();
			private:
				std::string m_utf8URL;
				CefRefPtr<ClientHandler> m_clientHandler;
			};
		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD