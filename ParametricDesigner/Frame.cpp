#include "Frame.h"
#include "EnumID.h"
#include "WebView/Widget.h"

wxBEGIN_EVENT_TABLE(PFD::GUI::Frame, wxFrame)
	EVT_CLOSE(PFD::GUI::Frame::OnClose)
	EVT_TIMER(ID_TIMER, PFD::GUI::Frame::OnTimer)
	EVT_MENU(LAYOUT_ABOUT, PFD::GUI::Frame::OnAbout)
	EVT_MENU(ID_OPEN_GLTF, PFD::GUI::Frame::OnOpenGLTF)
	EVT_MENU(ID_EXPORT_GLTF, PFD::GUI::Frame::OnExportSence)
	EVT_MENU(ID_CLOSE_GLTF, PFD::GUI::Frame::OnCloseGLTF)
	EVT_MENU(ID_EXIT_PROGRAM, PFD::GUI::Frame::OnExit)
wxEND_EVENT_TABLE()

namespace PFD
{
	namespace GUI
	{
		Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
			: wxFrame(nullptr, wxID_ANY, title, pos, size)
		{
			SetIcon(wxICON(sample));

			SetMinSize(wxSize(1500, 1000));

			wxMenuBar* menu_bar = new wxMenuBar;
			SetMenuBar(menu_bar);

			wxMenu* file_menu = new wxMenu;
			menu_bar->Append(file_menu, "&File");
			file_menu->Append(ID_OPEN_GLTF, "&Open");
			file_menu->Append(ID_EXPORT_GLTF, "&Save");
			file_menu->Append(ID_CLOSE_GLTF, "&Close");
			file_menu->Append(ID_EXIT_PROGRAM, "&Exit");

			wxMenu* help_menu = new wxMenu;
			menu_bar->Append(help_menu, "&Help");
			help_menu->Append(LAYOUT_ABOUT, "&About", "About layout demo...");

			CreateStatusBar();
			SetStatusText("Parametric Designer CopyRight");
			wxGridSizer* topsizer = new wxGridSizer(2, 2, 1, 1);

			WebView::Widget* pWebView = new WebView::Widget(this, "");
			topsizer->Add(pWebView);

			topsizer->SetMinSize(wxSize(1208, 1000));
			topsizer->SetHGap(FromDIP(1));
			topsizer->SetVGap(FromDIP(1));
			Init3DWindow();

			topsizer->Add(m_p3DWindow);



			SetSizer(topsizer);
		}

		void Frame::OnClose(wxCloseEvent& event)
		{
			// TODO
			PFD::Scene::IManager::GetInstance()->StopRender();
			Destroy();
		}

		void Frame::OnTimer(wxTimerEvent& event)
		{
		}

		void Frame::OnAbout(wxCommandEvent& event)
		{
			wxMessageBox("About Parametric Designer", "About");
		}

		void Frame::OnOpenGLTF(wxCommandEvent& event)
		{
			wxFileDialog dialog(this, "Please choose an GLTF",
				wxEmptyString, wxEmptyString, "*.gltf;*.glb", wxFD_OPEN);

			if (dialog.ShowModal() == wxID_OK)
			{
				wxString filename(dialog.GetPath());
				PFD::Scene::IManager::GetInstance()->ImportScene(filename.ToStdWstring());
			}
		}

		void Frame::OnCloseGLTF(wxCommandEvent& event)
		{
			PFD::Scene::IManager::GetInstance()->CloseScene();
		}

		void Frame::OnExit(wxCommandEvent& event)
		{
			// TODO
			PFD::Scene::IManager::GetInstance()->StopRender();
			Destroy();
		}

		void Frame::OnExportSence(wxCommandEvent& event)
		{
			wxFileDialog dialog(this,
				"Save Scene",
				wxEmptyString,
				wxEmptyString,
				/*"*.gltf;|*.glb;",*/
				"GLTF files (*.gltf)|*.gltf|GLB files (*.glb)|*.glb",
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

			if (dialog.ShowModal() == wxID_OK)
			{
				wxString filename = dialog.GetPath();
				PFD::Scene::IManager::GetInstance()->ExportScene(filename.ToStdWstring());
			}

		}

		void Frame::Init3DWindow()
		{
			m_p3DWindow = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(900), FromDIP(900)));

			if (!m_p3DWindow)
			{
				wxASSERT(m_p3DWindow);
				return;
			}

			HWND hwndDrawing = m_p3DWindow->GetHWND();
			PFD::Scene::IManager::GetInstance()->SetHWND(hwndDrawing);
			PFD::Scene::IManager::GetInstance()->BeginRender();
		}

	}
}

