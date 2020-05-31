#include "Widget.h"

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			Widget::Widget()
			{
			}

			Widget::Widget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
				: wxControl(parent, id, pos, size, style, validator, name)
			{
				Bind(wxEVT_SIZE, &Widget::OnSize, this);
			}

			Widget::~Widget()
			{
			}

			bool Widget::LoadURL(const std::string& utf8URL)
			{
				CefRefPtr<CefBrowser> pCefBrowser = m_clientHandler->GetBrowser();
				if (!pCefBrowser)
				{
					return false;
				}

				CefRefPtr<CefFrame> pCefFrame = pCefBrowser->GetMainFrame();
				if (!pCefFrame)
				{
					return false;
				}

				CefString cefStrURL(utf8URL);
				pCefFrame->LoadURL(cefStrURL);
				return true;
			}

			void Widget::OnSize(wxSizeEvent& event)
			{
				wxSize size = GetClientSize();
				if (m_clientHandler && m_clientHandler->GetBrowser() && m_clientHandler->GetBrowser()->GetHost())
				{
					HWND handle = m_clientHandler->GetBrowser()->GetHost()->GetWindowHandle();
					if (handle)
					{
						HDWP hdwp = BeginDeferWindowPos(1);
						hdwp = DeferWindowPos(hdwp, handle, NULL, 0, 0,
							size.GetWidth(), size.GetHeight(), SWP_NOZORDER);
						EndDeferWindowPos(hdwp);
					}
				}
				event.Skip();
			}
		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD