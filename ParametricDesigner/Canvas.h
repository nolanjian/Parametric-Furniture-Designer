#pragma once
#include "WXIncluding.h"
#include <osgViewer/GraphicsWindow>

namespace PFDGUI {
	class Canvas : public wxGLCanvas
	{
	public:
		explicit Canvas(wxWindow* parent,
			wxWindowID id = wxID_ANY,
			const int* attribList = NULL,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = 0,
			const wxString& name = wxGLCanvasName,
			const wxPalette& palette = wxNullPalette);
		virtual ~Canvas();

		void SetGraphicsWindow(osgViewer::GraphicsWindow* gw) { m_pGraphicsWindow = gw; }

		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnEraseBackground(wxEraseEvent& event);

		void OnChar(wxKeyEvent& event);
		void OnKeyUp(wxKeyEvent& event);

		void OnMouseEnter(wxMouseEvent& event);
		void OnMouseDown(wxMouseEvent& event);
		void OnMouseUp(wxMouseEvent& event);
		void OnMouseMotion(wxMouseEvent& event);
		void OnMouseWheel(wxMouseEvent& event);

		void UseCursor(bool value);

		void SetContextCurrent();
	private:
		wxDECLARE_NO_COPY_CLASS(PFDGUI::Canvas);
		DECLARE_EVENT_TABLE()

		osg::ref_ptr<osgViewer::GraphicsWindow> m_pGraphicsWindow;

		wxCursor m_oldCursor;
		wxGLContext m_context;
	};
};