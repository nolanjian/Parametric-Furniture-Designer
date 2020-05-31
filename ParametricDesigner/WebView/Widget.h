#pragma once
#include "wx/control.h"
#include "ClientHandler.h"
#include <string>

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			class Widget : public wxControl
			{
			public:
				explicit Widget();
				explicit Widget(wxWindow* parent, wxWindowID id = wxID_ANY,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize, long style = 0,
					const wxValidator& validator = wxDefaultValidator,
					const wxString& name = wxControlNameStr);
				virtual ~Widget();

				bool LoadURL(const std::string& utf8URL);
			protected:
				void OnSize(wxSizeEvent& event);
			private:
				CefRefPtr<ClientHandler> m_clientHandler;
			};
		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD