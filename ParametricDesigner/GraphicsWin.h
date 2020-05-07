#pragma once

#include <osgViewer/Viewer>

#include "Canvas.h"

namespace PFDGUI {
	class GraphicsWin : public osgViewer::GraphicsWindow
	{
	public:
		GraphicsWin(Canvas* canvas);
		~GraphicsWin();

		void init();

		//
		// GraphicsWindow interface
		//
		void grabFocus();
		void grabFocusIfPointerInWindow();
		void useCursor(bool cursorOn);

		bool makeCurrentImplementation();
		void swapBuffersImplementation();

		// not implemented yet...just use dummy implementation to get working.
		virtual bool valid() const { return true; }
		virtual bool realizeImplementation() { return true; }
		virtual bool isRealizedImplementation() const { return m_pCanvas->IsShownOnScreen(); }
		virtual void closeImplementation() {}
		virtual bool releaseContextImplementation() { return true; }
	private:
		Canvas* m_pCanvas;
	};
};