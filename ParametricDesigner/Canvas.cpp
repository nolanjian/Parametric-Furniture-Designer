#include "Canvas.h"

BEGIN_EVENT_TABLE(PFDGUI::Canvas, wxGLCanvas)
	EVT_SIZE(PFDGUI::Canvas::OnSize)
	EVT_PAINT(PFDGUI::Canvas::OnPaint)
	EVT_ERASE_BACKGROUND(PFDGUI::Canvas::OnEraseBackground)

	EVT_CHAR(PFDGUI::Canvas::OnChar)
	EVT_KEY_UP(PFDGUI::Canvas::OnKeyUp)

	EVT_ENTER_WINDOW(PFDGUI::Canvas::OnMouseEnter)
	EVT_LEFT_DOWN(PFDGUI::Canvas::OnMouseDown)
	EVT_MIDDLE_DOWN(PFDGUI::Canvas::OnMouseDown)
	EVT_RIGHT_DOWN(PFDGUI::Canvas::OnMouseDown)
	EVT_LEFT_UP(PFDGUI::Canvas::OnMouseUp)
	EVT_MIDDLE_UP(PFDGUI::Canvas::OnMouseUp)
	EVT_RIGHT_UP(PFDGUI::Canvas::OnMouseUp)
	EVT_MOTION(PFDGUI::Canvas::OnMouseMotion)
	EVT_MOUSEWHEEL(PFDGUI::Canvas::OnMouseWheel)
END_EVENT_TABLE()

PFDGUI::Canvas::Canvas(wxWindow* parent, wxWindowID id, const int* attribList, const wxPoint& pos, const wxSize& size, long style, const wxString& name, const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
	, m_oldCursor(*wxSTANDARD_CURSOR)
	, m_context(this)
{
}

PFDGUI::Canvas::~Canvas()
{
}

void PFDGUI::Canvas::OnPaint(wxPaintEvent& event)
{
	/* must always be here */
	wxPaintDC dc(this);
}

void PFDGUI::Canvas::OnSize(wxSizeEvent& event)
{    
	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int width, height;
	GetClientSize(&width, &height);

	if (m_pGraphicsWindow.valid())
	{
		// update the window dimensions, in case the window has been resized.
		m_pGraphicsWindow->getEventQueue()->windowResize(0, 0, width, height);
		m_pGraphicsWindow->resized(0, 0, width, height);
	}
}

void PFDGUI::Canvas::OnEraseBackground(wxEraseEvent& event)
{
	/* Do nothing, to avoid flashing on MSW */
}

void PFDGUI::Canvas::OnChar(wxKeyEvent& event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif

	if (m_pGraphicsWindow.valid())
		m_pGraphicsWindow->getEventQueue()->keyPress(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}

void PFDGUI::Canvas::OnKeyUp(wxKeyEvent& event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif

	if (m_pGraphicsWindow.valid())
		m_pGraphicsWindow->getEventQueue()->keyRelease(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}

void PFDGUI::Canvas::OnMouseEnter(wxMouseEvent& event)
{    
	// Set focus to ourselves, so keyboard events get directed to us
	SetFocus();
}

void PFDGUI::Canvas::OnMouseDown(wxMouseEvent& event)
{
	if (m_pGraphicsWindow.valid())
	{
		m_pGraphicsWindow->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(),
			event.GetButton());
	}
}

void PFDGUI::Canvas::OnMouseUp(wxMouseEvent& event)
{
	if (m_pGraphicsWindow.valid())
	{
		m_pGraphicsWindow->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(),
			event.GetButton());
	}
}

void PFDGUI::Canvas::OnMouseMotion(wxMouseEvent& event)
{
	if (m_pGraphicsWindow.valid())
		m_pGraphicsWindow->getEventQueue()->mouseMotion(event.GetX(), event.GetY());
}

void PFDGUI::Canvas::OnMouseWheel(wxMouseEvent& event)
{
	int delta = event.GetWheelRotation() / event.GetWheelDelta() * event.GetLinesPerAction();

	if (m_pGraphicsWindow.valid()) {
		m_pGraphicsWindow->getEventQueue()->mouseScroll(
			delta > 0 ?
			osgGA::GUIEventAdapter::SCROLL_UP :
			osgGA::GUIEventAdapter::SCROLL_DOWN);
	}
}

void PFDGUI::Canvas::UseCursor(bool value)
{
	if (value)
	{
		// show the old cursor
		SetCursor(m_oldCursor);
	}
	else
	{
		// remember the old cursor
		m_oldCursor = GetCursor();

		// hide the cursor
		//    - can't find a way to do this neatly, so create a 1x1, transparent image
		wxImage image(1, 1);
		image.SetMask(true);
		image.SetMaskColour(0, 0, 0);
		wxCursor cursor(image);
		SetCursor(cursor);

		// On wxGTK, only works as of version 2.7.0
		// (http://trac.wxwidgets.org/ticket/2946)
		// SetCursor( wxStockCursor( wxCURSOR_BLANK ) );
	}
}

void PFDGUI::Canvas::SetContextCurrent()
{
	m_context.SetCurrent(*this);
}
