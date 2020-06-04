#include "Frame.h"
#include "EnumID.h"
#include "../WebView/Widget.h"
#include "../Config/IProgramConfig.h"

wxBEGIN_EVENT_TABLE(PFD::GUI::Frame, wxFrame)
	EVT_CLOSE(PFD::GUI::Frame::OnClose)
	EVT_TIMER(ID_TIMER, PFD::GUI::Frame::OnTimer)
	EVT_MENU(LAYOUT_ABOUT, PFD::GUI::Frame::OnAbout)
	EVT_MENU(ID_OPEN_GLTF, PFD::GUI::Frame::OnOpenGLTF)
	EVT_MENU(ID_EXPORT_GLTF, PFD::GUI::Frame::OnExportSence)
	EVT_MENU(ID_CLOSE_GLTF, PFD::GUI::Frame::OnCloseGLTF)
	EVT_MENU(ID_EXIT_PROGRAM, PFD::GUI::Frame::OnExit)
wxEND_EVENT_TABLE()

//#define USE_WEB_VIEW_MENU_BAR

namespace PFD
{
	namespace GUI
	{
		Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
			: wxFrame(nullptr, wxID_ANY, title, pos, size
				//, wxRESIZE_BORDER | wxCLIP_CHILDREN
			)
		{
			SetIcon(wxICON(sample));
			nlohmann::json jsonGUI = PFD::Config::IProgramConfig::GetInstance()->GetJson("GUI");
			nlohmann::json jsonMainFrameMinSize = jsonGUI["MainFrameMinSize"];
			int nX = jsonMainFrameMinSize["X"].get<int>();
			int nY = jsonMainFrameMinSize["Y"].get<int>();
			SetMinSize(wxSize(nX, nY));

#ifdef USE_WEB_VIEW_MENU_BAR

#else
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
#endif // USE_NATIVE_MENU_BAR

			CreateStatusBar();
			SetStatusText("Parametric Designer CopyRight");

			wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
			//topSizer->SetMinSize(wxSize(1208, 1000));

			WebView::Widget* pToolbar = new WebView::Widget(this, "");
			topSizer->Add(pToolbar);
			pToolbar->SetMinSize(wxSize(nX, 200));

			wxBoxSizer* pViewBar = new wxBoxSizer(wxHORIZONTAL);
			topSizer->Add(pViewBar);
			pViewBar->SetMinSize(wxSize(300, 900));

			WebView::Widget* pLeftView = new WebView::Widget(this, "");
			pViewBar->Add(pLeftView);

#ifdef USING_WEBGL
			WebView::Widget* pWebGLView = new WebView::Widget(this, "");
			pViewBar->Add(pWebGLView);
#else
			Init3DWindow();
			pViewBar->Add(m_p3DWindow);
#endif // USING_WEBGL

			SetSizer(topSizer);
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
			nlohmann::json jsonGUI = PFD::Config::IProgramConfig::GetInstance()->GetJson("GUI");
			nlohmann::json jsonMainFrameMinSize = jsonGUI["3DWindowMinSize"];
			int nX = jsonMainFrameMinSize["X"].get<int>();
			int nY = jsonMainFrameMinSize["Y"].get<int>();
			SetMinSize(wxSize(nX, nY));

			m_p3DWindow = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(nX, nY));

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

