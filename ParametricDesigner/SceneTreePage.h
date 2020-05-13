#pragma once

#include "WXIncluding.h"

namespace PFDGUI
{
	class SceneTreePage : public wxPanel
	{
	public:
		SceneTreePage(wxWindow* parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
		virtual ~SceneTreePage();

		void Update();
		void UpdateTreeCtrl();

	protected:
	private:
		wxTreeCtrl* m_pTreeCtrl = nullptr;
	};
}