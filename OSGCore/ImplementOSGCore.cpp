#include "pch.h"
#include "ImplementOSGCore.h"
#include <cassert>
#include <osg/GraphicsContext>
#include <osgViewer/Viewer>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

OSGCore::ImplementOSGCore::ImplementOSGCore()
{
}

OSGCore::ImplementOSGCore::~ImplementOSGCore()
{
	Destory();
}

bool OSGCore::ImplementOSGCore::Render(HWND hwnd)
{
	assert(hwnd != nullptr);
	if (hwnd == NULL)
	{
		return false;
	}
	else if (m_hwnd == NULL)
	{
		m_hwnd = hwnd;
	}
	else if (m_hwnd == hwnd)
	{
		return true;
	}
	else
	{
		s_bKeepRunning = false;
		Destory();
	}

	RECT rect;
	GetWindowRect(m_hwnd, &rect);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hwnd);
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->inheritedWindowData = windata;
	traits->pbuffer = false;

	// We must set the pixelformat before we can create the OSG Rendering Surface 
	PIXELFORMATDESCRIPTOR pixelFormat =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Specifies the size of this data structure. This value should be set to sizeof(PIXELFORMATDESCRIPTOR). 
		1,	// Specifies the version of this data structure. This value should be set to 1. 
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // A set of bit flags that specify properties of the pixel buffer 
		PFD_TYPE_RGBA, // Specifies the type of pixel data.  PFD_TYPE_RGBA or PFD_TYPE_COLORINDEX 
		24,	// Specifies the number of color bitplanes in each color buffer. For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes 
		0,  // cRedBits	  Specifies the number of red bitplanes in each RGBA color buffer. 
		0,  // cRedShift  Specifies the shift count for red bitplanes in each RGBA color buffer. 
		0,0,// cGreenBits, cGreenShift 
		0,0,// cBlueBits , cBlueShift 
		0,0,// cAlphaBits, cAlphaShift 
		0,  // cAccumBits  Specifies the total number of bitplanes in the accumulation buffer. 
		0,  // cAccumRedBits Specifies the number of red bitplanes in the accumulation buffer. 
		0,0,0,// cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits 
		24,// cDepthBits     Specifies the depth of the depth (z-axis) buffer. 
		0, // cStencilBits   Specifies the depth of the stencil buffer. 
		0, // cAuxBuffers    Specifies the number of auxiliary buffers. Auxiliary buffers are not supported. 
		PFD_MAIN_PLANE, // iLayerType Ignored. Earlier implementations of OpenGL used this member, but it is no longer used. 
		0, // bReserved      Specifies the number of overlay and underlay planes. Bits 0 through 3 specify up to 15 overlay planes and bits 4 through 7 specify up to 15 underlay planes 
		0, // dwLayerMask    Ignored. Earlier implementations of OpenGL used this member, but it is no longer used. 
		0, // dwVisibleMask  Specifies the transparent color or index of an underlay plane. When the pixel type is RGBA, dwVisibleMask is a transparent RGB color value 
		0  // dwDamageMask   Ignored. Earlier implementations of OpenGL used this member, but it is no longer used. 
	};

	// Retrieves a device context for the client area. (ATL) 
	HDC hdc = ::GetDC(m_hwnd);
	if (hdc == 0)
	{
		// Destroys the window associated with the CWindow object and sets m_hWnd to NULL. 
		::DestroyWindow(m_hwnd);
		return false;
	}

	// The ChoosePixelFormat OpenGL function attempts to match an appropriate pixel format supported by a device context to a given pixel format specification. 
	int pixelFormatIndex = ::ChoosePixelFormat(hdc, &pixelFormat);
	if (pixelFormatIndex == 0)
	{
		// (MFC)Releases the display device context of a container of a windowless control, freeing the device context for use by other applications 
		::ReleaseDC(m_hwnd, hdc);
		::DestroyWindow(m_hwnd);
		return false;
	}

	// The SetPixelFormat OpenGL function sets the pixel format of the specified device context to the format specified by the iPixelFormat index. 
	if (!::SetPixelFormat(hdc, pixelFormatIndex, &pixelFormat))
	{
		::ReleaseDC(m_hwnd, hdc);
		::DestroyWindow(m_hwnd);
		return false;
	}

	// Create graphics context 
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc.get());
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setDrawBuffer(GL_BACK);
	camera->setReadBuffer(GL_BACK);

	// Create the viewer and attach the camera to it 
	m_ptrViewer = new osgViewer::Viewer;
	m_ptrViewer->addSlave(camera.get());

	m_ptrViewer->setCamera(camera.get());
	m_ptrViewer->setSceneData(m_ptr3DScene);
	osg::Node*	pCow = osgDB::readNodeFile("D:\\library\\Parametric-Furniture-Designer\\OpenSceneGraph-Data-3.0.0\\SmokeBox.osgt");
	m_ptr3DScene->addChild(pCow);

	m_ptrViewer->setKeyEventSetsDone(0);
	m_ptrViewer->setCameraManipulator(new osgGA::TrackballManipulator);

	m_renderThread = std::thread(std::bind(&ImplementOSGCore::RenderThread, this));

	return true;
}

void OSGCore::ImplementOSGCore::Destory()
{
	s_bKeepRunning = false;
	if (m_ptrViewer.get())
	{
		m_ptrViewer->setDone(true);
	}
	if (m_renderThread.joinable())
	{
		m_renderThread.join();
	}
}

void OSGCore::ImplementOSGCore::RenderThread()
{
	s_bKeepRunning = true;
	m_ptrViewer->run();
	s_bKeepRunning = false;
}

void OSGCore::ImplementOSGCore::SetModelPath(const std::wstring& path)
{
	m_ptr3DScene->removeChildren(0, m_ptr3DScene->getNumChildren());
	std::string	modelPath;
	osg::Node* pModel = osgDB::readNodeFile(modelPath);
	if (pModel)
	{
		m_ptr3DScene->addChild(pModel);
	}

	if (fnOnSelectObjectCallback)
	{
		fnOnSelectObjectCallback(-1);
	}

}
