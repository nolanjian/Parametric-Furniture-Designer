#include "SceneTreePage.h"

namespace PFDGUI
{
	SceneTreePage::SceneTreePage(wxWindow* parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxPanel(parent, winid, pos, size, style, name)
	{
		UpdateTreeCtrl();
	}
	SceneTreePage::~SceneTreePage()
	{
		m_pTreeCtrl = nullptr;
	}
	void SceneTreePage::SetInterface3D(std::shared_ptr<PFDCore::Interface3D> interface3D)
	{
		m_pInterface3D = interface3D;
	}
	void SceneTreePage::Update()
	{
		if (m_pInterface3D)
		{
			m_pInterface3D->GetSenceTree();
		}
	}
	void SceneTreePage::UpdateTreeCtrl()
	{
		if (!m_pTreeCtrl)
		{
			wxSizer* pSizer = GetSizer();
			if (pSizer)
			{
				pSizer->GetChildren();
			}
		}
	}
}
