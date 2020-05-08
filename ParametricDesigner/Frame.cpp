#include "Frame.h"
#include "EnumID.h"

wxBEGIN_EVENT_TABLE(PFDGUI::Frame, wxFrame)
	EVT_CLOSE(PFDGUI::Frame::OnClose)
	EVT_TIMER(ID_TIMER, PFDGUI::Frame::OnTimer)
	EVT_MENU(LAYOUT_ABOUT, PFDGUI::Frame::OnAbout)
	EVT_MENU(ID_OPEN_GLTF, PFDGUI::Frame::OnOpenGLTF)
wxEND_EVENT_TABLE()

PFDGUI::Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	SetIcon(wxICON(sample));

	wxMenuBar* menu_bar = new wxMenuBar;
	SetMenuBar(menu_bar);

	wxMenu* file_menu = new wxMenu;
	menu_bar->Append(file_menu, "&File");
	file_menu->Append(ID_OPEN_GLTF, "&Open");

	wxMenu* help_menu = new wxMenu;
	menu_bar->Append(help_menu, "&Help");
	help_menu->Append(LAYOUT_ABOUT, "&About", "About layout demo...");

	CreateStatusBar();
	SetStatusText("Parametric Designer CopyRight");

	wxGridSizer* topsizer = new wxGridSizer(2, 2, 1, 1);
	
	wxWindow* pDrawWin = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(600), FromDIP(600)));
	topsizer->Add(pDrawWin);

	HWND hwndDrawing = pDrawWin->GetHWND();

	m_pInterfaceOSG = std::make_shared<PFDCore::InterfaceOSG>();
	m_pInterfaceOSG->Render(hwndDrawing);

	SetSizer(topsizer);
}

void PFDGUI::Frame::OnClose(wxCloseEvent& event)
{
	// TODO
	Destroy();
}

void PFDGUI::Frame::OnTimer(wxTimerEvent& event)
{
}

void PFDGUI::Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("About Parametric Designer", "About");
}

void PFDGUI::Frame::OnOpenGLTF(wxCommandEvent& event)
{
	wxFileDialog dialog(this, "Please choose an GLTF",
		wxEmptyString, wxEmptyString, "*.gltf;*.glb", wxFD_OPEN);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString filename(dialog.GetPath());
		if (m_pInterfaceOSG)
		{
			m_pInterfaceOSG->LoadScene(filename.c_str().AsChar());
		}
	}
}
