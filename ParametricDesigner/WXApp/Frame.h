/*****************************************************************//**
 * \file   Frame.h
 * \brief  Frame Class For PFD App
 * 
 * \author NolanJian
 * \date   June 2020
 *********************************************************************/

#pragma once
#include "WXIncluding.h"
#include <SceneManager/Manager/IManager.h>
#include <memory>

namespace PFD
{
	namespace GUI
	{
		/**
		 * \brief  Frame Class Define
		 * \author NolanJian
		 * \date   2020/06/09
		 *  
		 * */
		class Frame : public wxFrame
		{
		public:
			Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
			virtual ~Frame();
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