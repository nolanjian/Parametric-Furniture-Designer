#pragma once

#include "WXIncluding.h"
#include "Interface3D.h"

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

		void SetInterface3D(std::shared_ptr<PFDCore::Interface3D> interface3D);
		void Update();
		void UpdateTreeCtrl();

	protected:
	private:
		std::shared_ptr<PFDCore::Interface3D>	m_pInterface3D;

		wxTreeCtrl* m_pTreeCtrl = nullptr;
	};
}