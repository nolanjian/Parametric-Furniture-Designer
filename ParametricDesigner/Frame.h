#pragma once
#include "WXIncluding.h"
#include "Interface3D.h"
#include <memory>

namespace PFDGUI
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

		wxDECLARE_EVENT_TABLE();
	private:
		std::shared_ptr<PFDCore::Interface3D>	m_pInterfaceOSG;
	};
};