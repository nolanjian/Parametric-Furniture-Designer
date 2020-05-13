#include "SceneTreePage.h"
#include "ISceneManager.h"

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
	
	void SceneTreePage::Update()
	{
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
