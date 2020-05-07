#include "GraphicsWin.h"

PFDGUI::GraphicsWin::GraphicsWin(Canvas* canvas)
{
	m_pCanvas = canvas;

	_traits = new GraphicsContext::Traits;

	wxPoint pos = m_pCanvas->GetPosition();
	wxSize  size = m_pCanvas->GetSize();

	_traits->x = pos.x;
	_traits->y = pos.y;
	_traits->width = size.x;
	_traits->height = size.y;

	

	init();
}

PFDGUI::GraphicsWin::~GraphicsWin()
{
}

void PFDGUI::GraphicsWin::init()
{
	if (valid())
	{
		setState(new osg::State);
		getState()->setGraphicsContext(this);

		if (_traits.valid() && _traits->sharedContext.valid())
		{
			getState()->setContextID(_traits->sharedContext->getState()->getContextID());
			incrementContextIDUsageCount(getState()->getContextID());
		}
		else
		{
			getState()->setContextID(osg::GraphicsContext::createNewContextID());
		}
	}
}

void PFDGUI::GraphicsWin::grabFocus()
{
	m_pCanvas->SetFocus();
}

void PFDGUI::GraphicsWin::grabFocusIfPointerInWindow()
{
	// focus this window, if the pointer is in the window
	wxPoint pos = wxGetMousePosition();
	if (wxFindWindowAtPoint(pos) == m_pCanvas)
		m_pCanvas->SetFocus();
}

void PFDGUI::GraphicsWin::useCursor(bool cursorOn)
{
	m_pCanvas->UseCursor(cursorOn);
}

bool PFDGUI::GraphicsWin::makeCurrentImplementation()
{
	m_pCanvas->SetContextCurrent();
	return true;
}

void PFDGUI::GraphicsWin::swapBuffersImplementation()
{
	m_pCanvas->SwapBuffers();
}
