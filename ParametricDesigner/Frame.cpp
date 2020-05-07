#include "Frame.h"
#include "EnumID.h"
#include "Canvas.h"
#include "GraphicsWin.h"

wxBEGIN_EVENT_TABLE(PFDGUI::Frame, wxFrame)
	EVT_CLOSE(PFDGUI::Frame::OnClose)
	EVT_TIMER(ID_TIMER, PFDGUI::Frame::OnTimer)
	EVT_MENU(LAYOUT_ABOUT, PFDGUI::Frame::OnAbout)
wxEND_EVENT_TABLE()

PFDGUI::Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	SetIcon(wxICON(sample));

	//CreateCanvas();

	wxMenuBar* menu_bar = new wxMenuBar;
	SetMenuBar(menu_bar);

	wxMenu* file_menu = new wxMenu;
	menu_bar->Append(file_menu, "&File");

	wxMenu* help_menu = new wxMenu;
	menu_bar->Append(help_menu, "&Help");
	help_menu->Append(LAYOUT_ABOUT, "&About", "About layout demo...");

	CreateStatusBar();
	SetStatusText("Parametric Designer CopyRight");

	wxGridSizer* topsizer = new wxGridSizer(2, 2, 1, 1);

	topsizer->SetMinSize(wxSize(800, 800));
	
	// Tell dialog to use sizer
	SetSizerAndFit(topsizer);
	
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

void PFDGUI::Frame::CreateCanvas()
{
	int attributes[7];
	attributes[0] = int(WX_GL_DOUBLEBUFFER);
	attributes[1] = WX_GL_RGBA;
	attributes[2] = WX_GL_DEPTH_SIZE;
	attributes[3] = 8;
	attributes[4] = WX_GL_STENCIL_SIZE;
	attributes[5] = 8;
	attributes[6] = 0;

	int width = 800, height = 800;

	Canvas* canvas = new Canvas(this, wxID_ANY, attributes, wxDefaultPosition, wxSize(width, height), wxSUNKEN_BORDER, wxT("osgviewerWX"));
	GraphicsWin* gw = new GraphicsWin(canvas);

	canvas->SetGraphicsWindow(gw);

	osgViewer::Viewer* viewer = new osgViewer::Viewer;
	viewer->getCamera()->setGraphicsContext(gw);
	viewer->getCamera()->setViewport(0, 0, width, height);

	// set the draw and read buffers up for a double buffered window with rendering going to back buffer
	viewer->getCamera()->setDrawBuffer(GL_BACK);
	viewer->getCamera()->setReadBuffer(GL_BACK);

	//viewer->addEventHandler(new osgViewer::StatsHandler);
	viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	//viewer->setSceneData(loadedModel.get());
	//viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	//frame->SetViewer(viewer);

	
}


