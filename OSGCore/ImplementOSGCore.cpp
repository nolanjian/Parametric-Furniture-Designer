#include "pch.h"
#include "ImplementOSGCore.h"
#include "BaseObject.h"
#include "TestOSGWin.h"
#include "ShadingPreDefine.h"
#include "../Utils/Utils.h"

PFDCore::ImplementOSGCore::ImplementOSGCore()
{
	InitLogger();

	LOG(INFO) << "ImplementOSGCore()";
}

PFDCore::ImplementOSGCore::~ImplementOSGCore()
{
	Destory();

	LOG(INFO) << "~ImplementOSGCore()";
}

bool PFDCore::ImplementOSGCore::Render(HWND hwnd)
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

	initViewer(osg::GraphicsContext::createGraphicsContext(traits.get()));

	auto scene = PFDCore::BaseObject::LoadSceneFromJsonFile("D:\\glTF-Sample-Models\\2.0\\Lantern\\glTF\\Lantern.gltf");
	if (scene)
	{
		//osgUtil::Optimizer optimizer;
		//optimizer.optimize(scene.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS | osgUtil::Optimizer::TESSELLATE_GEOMETRY);
		configureShaders(scene->getOrCreateStateSet());
		m_ptrViewer->setSceneData(scene);
	}

	m_renderThread = std::thread(std::bind(&ImplementOSGCore::RenderThread, this));
	return true;
}

void PFDCore::ImplementOSGCore::Destory()
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

void PFDCore::ImplementOSGCore::RenderThread()
{
	s_bKeepRunning = true;
	m_ptrViewer->run();
	s_bKeepRunning = false;
}

void PFDCore::ImplementOSGCore::SetModelPath(const std::wstring& path)
{
	m_ptr3DScene->removeChildren(0, m_ptr3DScene->getNumChildren());

	std::string strPath;
	m_ptr3DScene = PFDCore::BaseObject::LoadSceneFromJsonFile(strPath);

	if (fnOnSelectObjectCallback)
	{
		fnOnSelectObjectCallback(-1);
	}

}

void PFDCore::ImplementOSGCore::LoadScene(const std::string& path)
{
	PFDCore::BaseObject::LoadSceneFromJsonFile(path);
}

osg::ref_ptr<osg::Program> createProgram()
{
	return nullptr;
}

void PFDCore::ImplementOSGCore::OpenSceneInNewWindow(const std::string& path)
{
	TestOSGWin* pWin = new TestOSGWin();
	pWin->Run(path);
	delete pWin;
	pWin = nullptr;
	return;
}

std::string LoadShaderString(const std::string& path)
{
	std::ifstream	fs(path);
	std::stringstream	ss;
	ss << fs.rdbuf();
	std::string source = ss.str();
	//LOG(INFO) << source;
	return source;
}

void PFDCore::ImplementOSGCore::configureShaders(osg::StateSet* stateSet)
{
	if (!stateSet)
	{
		return;
	}

	osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, LoadShaderString("D:\\library\\Parametric-Furniture-Designer\\OSGCore\\VERTEX.vert"));
	osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, LoadShaderString("D:\\library\\Parametric-Furniture-Designer\\OSGCore\\FRAGMENT.frag"));

	osg::Program* program = new osg::Program;
	program->addShader(vShader);
	program->addShader(fShader);
	program->addBindAttribLocation(TANGENT, TANGENT_INDEX);
	stateSet->setAttribute(program);

	osg::Vec3f lightDir(0., 0.5, 1.);
	lightDir.normalize();
	stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));
}

void PFDCore::ImplementOSGCore::initViewer(osg::ref_ptr<osg::GraphicsContext> gc)
{
	m_ptrViewer = new osgViewer::Viewer;
	osg::Camera* cam = m_ptrViewer->getCamera();
	cam->setProjectionMatrix(osg::Matrix::perspective(30., (double)800 / (double)600, 1., 100.));
	cam->setClearColor(osg::Vec4(94.0 / 255.0, 112 / 255.0, 129 / 255.0, 1.0));
	cam->setGraphicsContext(gc);
	cam->setViewport(new osg::Viewport(0, 0, gc->getTraits()->width, gc->getTraits()->height));
	cam->setDrawBuffer(GL_BACK);
	cam->setReadBuffer(GL_BACK);

	osgViewer::Viewer::Windows windows;
	m_ptrViewer->getWindows(windows);
	for (osgViewer::Viewer::Windows::iterator itr = windows.begin();
		itr != windows.end();
		++itr)
	{
		osg::State* s = (*itr)->getState();
		s->setUseModelViewAndProjectionUniforms(true);
		s->setUseVertexAttributeAliasing(true);
	}
}
