#pragma once
#include "WXIncluding.h"
#include "InterfaceOSGCore.h"
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

		wxDECLARE_EVENT_TABLE();
	private:
		std::shared_ptr<PFDCore::InterfaceOSG>	m_pInterfaceOSG;
	};
};