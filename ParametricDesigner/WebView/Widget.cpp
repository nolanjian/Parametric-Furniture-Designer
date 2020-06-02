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

			Widget::Widget(wxWindow* parent, const std::string& utf8URL, wxWindowID id, const wxPoint& pos, const wxSize& size
				, long style, const wxValidator& validator, const wxString& name)
				: wxControl(parent, id, pos, size, style, validator, name)
				, m_utf8URL(utf8URL)
			{
				InitCEF();
				InitEvents();
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

			void Widget::CloseCefBrowser()
			{
				CefRefPtr<CefBrowser> browser = m_clientHandler->GetBrowser();
				if (browser.get()) {
					browser->GetHost()->CloseBrowser(true);
				}
			}

			void Widget::RunScript(const std::string& utf8Script)
			{
				if (!m_clientHandler)
				{
					return;
				}
				CefRefPtr<CefBrowser> pCefBrowser = m_clientHandler->GetBrowser();
				if (!pCefBrowser)
				{
					return;
				}
				CefRefPtr<CefFrame> pCefFrame = pCefBrowser->GetMainFrame();
				if (!pCefFrame)
				{
					return;
				}
				pCefFrame->ExecuteJavaScript(utf8Script, "", 0);
			}

			void Widget::InitCEF()
			{
				m_clientHandler = new ClientHandler(this);

				CefBrowserSettings browsersettings;
				CefWindowInfo info;

				wxRect rect = GetRect();
				RECT rct;
				rct.left = rect.GetLeft();
				rct.right = rect.GetRight();
				rct.top = rect.GetTop();
				rct.bottom = rect.GetBottom();
				info.SetAsChild(GetHWND(), rct);

				CefBrowserHost::CreateBrowser(info
					, static_cast<CefRefPtr<CefClient>>(m_clientHandler)
					, m_utf8URL, browsersettings
					, nullptr, nullptr);
			}

			void Widget::InitEvents()
			{
				Bind(wxEVT_SIZE, &Widget::OnSize, this);
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