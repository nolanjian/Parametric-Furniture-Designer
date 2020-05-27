#pragma once
#include "WXIncluding.h"
#include "IManager.h"
#include <memory>

namespace PFD
{
	namespace GUI
	{
		class Frame : public wxFrame
		{
		public:
			Frame(const wxString& title, const wxPoint& pos, const wxSize& size);

			// event handlers (these functions should _not_ be virtual)
			void OnClose(wxCloseEvent& event);
			void OnTimer(wxTimerEvent& event);
			void OnAbout(wxCommandEvent& event);
			void OnOpenGLTF(wxCommandEvent& event);
			void OnCloseGLTF(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnExportSence(wxCommandEvent& event);

		protected:
			void Init3DWindow();

			wxDECLARE_EVENT_TABLE();
		private:
			wxWindow* m_p3DWindow = nullptr;
		};
	}
}