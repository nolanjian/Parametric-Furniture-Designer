#include "Manager.h"
#include "IProgramConfig.h"
#include "Importer.h"
#include "Exporter.h"
#include "ShadingPreDefine.h"

namespace PFD
{
	namespace Scene
	{


		Manager::Manager()
		{

		}

		Manager::~Manager()
		{
			StopRender();
			m_p3DViewer = nullptr;
		}

		void Manager::SetHWND(HWND hwnd)
		{
			m_hwnd = hwnd;
		}
		HWND Manager::GetHWND()
		{
			return m_hwnd;
		}

		bool Manager::BeginRender()
		{
			if (m_hwnd == NULL)
			{
				logger->error("hwnd is null");
				return false;
			}

			RECT rect;
			if (GetWindowRect(m_hwnd, &rect) == FALSE)
			{
				logger->error("GetWindowRect fail");
				return false;
			}

			StopRender();

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
			//traits->sampleBuffers = 1;
			//traits->samples = 4;

			auto n =
				osg::DisplaySettings::instance()->getNumMultiSamples();

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
				logger->error("GetDC fail");
				return false;
			}

			// The ChoosePixelFormat OpenGL function attempts to match an appropriate pixel format supported by a device context to a given pixel format specification. 
			int pixelFormatIndex = ::ChoosePixelFormat(hdc, &pixelFormat);
			if (pixelFormatIndex == 0)
			{
				// (MFC)Releases the display device context of a container of a windowless control, freeing the device context for use by other applications 
				::ReleaseDC(m_hwnd, hdc);
				logger->error("ChoosePixelFormat fail");
				return false;
			}

			// The SetPixelFormat OpenGL function sets the pixel format of the specified device context to the format specified by the iPixelFormat index. 
			if (!::SetPixelFormat(hdc, pixelFormatIndex, &pixelFormat))
			{
				::ReleaseDC(m_hwnd, hdc);
				logger->error("SetPixelFormat fail");
				return false;
			}

			InitViewer(osg::GraphicsContext::createGraphicsContext(traits.get()));

			m_renderThread = std::thread(std::bind(&Manager::RenderThread, this));
			return true;
		}

		bool Manager::StopRender()
		{
			if (m_p3DViewer.get() && !m_p3DViewer->done())
			{
				m_p3DViewer->setDone(true);
			}
			if (m_renderThread.joinable())
			{
				m_renderThread.join();
				m_p3DViewer->stopThreading();
			}
			return true;
		}

		void Manager::SetRenderMode(RenderMode mode)
		{
			switch (mode)
			{
			case IManager::Texture:
				// TODO
				break;
			case IManager::WireFrames:
				// TODO
				break;
			case IManager::Cartoon:
				// TODO
				break;
			case IManager::BlackAndWhite:
				// TODO
				break;
			default:
				logger->error("Unknown Render Mode:{}", mode);
				return;
			}

			m_renderMode = mode;
		}

		IManager::RenderMode Manager::GetRenderMode()
		{
			return m_renderMode;
		}

		bool Manager::ImportScene(const std::wstring& path)
		{
			try
			{

				auto pImporter = std::make_shared<PFD::GLTF::Importer>();
				if (!pImporter)
				{
					logger->error("Create GLTF importer fail");
					return false;
				}

				osg::ref_ptr<osg::Group> pScene = pImporter->Load(path);
				if (!pScene)
				{
					logger->error("Load GLTF {} fail", PFD::Utils::WStringToString(path));
					return false;
				}

				bool bRet = OpenScene(pScene);
				return bRet;


			}
			catch (const std::exception& ex)
			{
				logger->error(ex.what());
				return false;
			}
		}

		bool Manager::ImportScene(const std::string& utf8Json)
		{
			auto pImporter = std::make_shared<PFD::GLTF::Importer>();
			if (!pImporter)
			{
				logger->error("Create GLTF importer fail");
				return false;
			}

			osg::ref_ptr<osg::Group> pScene = pImporter->Load(utf8Json);
			if (!pScene)
			{
				logger->error("Load GLTF {} fail", utf8Json);
				return false;
			}

			return OpenScene(pScene);
		}

		bool Manager::ExportScene(const std::wstring& path)
		{
			auto pExporter = std::make_shared<PFD::GLTF::Exporter>();
			if (pExporter)
			{
				osg::Node* pData = m_p3DViewer->getSceneData();
				if (pData)
				{
					osg::Group* pGrp = pData->asGroup();
					if (pGrp)
					{
						pExporter->Export(pGrp, path);
						return true;
					}
				}
			}

			return false;
		}

		bool Manager::ExportScene(std::string& utf8Json)
		{
			auto pExporter = std::make_shared<PFD::GLTF::Exporter>();
			if (pExporter)
			{
				osg::Node* pData = m_p3DViewer->getSceneData();
				if (pData)
				{
					osg::Group* pGrp = pData->asGroup();
					if (pGrp)
					{
						pExporter->Export(pGrp, utf8Json);
						return true;
					}
				}
			}

			return false;
		}

		bool Manager::OpenScene(osg::ref_ptr<osg::Group> pScene)
		{
			if (m_p3DViewer)
			{
				CloseScene();
				//m_p3DViewer->addEventHandler(new osgViewer::WindowSizeHandler);
				m_p3DViewer->addEventHandler(new osgViewer::StatsHandler());
				ConfigureShaders(pScene->getOrCreateStateSet());
				m_p3DViewer->setSceneData(pScene);
				return true;
			}
			return false;
		}

		bool Manager::CloseScene()
		{
			if (m_p3DViewer)
			{
				m_p3DViewer->setSceneData(nullptr);
			}
			return true;
		}

		bool Manager::EnableDepthTest(bool bEnable)
		{
			m_bEnableDepthTest = bEnable;

			osg::Node* pData = m_p3DViewer->getSceneData();
			if (pData)
			{
				pData->getOrCreateStateSet()->setMode(GL_DEPTH, m_bEnableDepthTest ? osg::StateAttribute::ON : osg::StateAttribute::OFF);
			}

			return m_bEnableDepthTest;
		}

		bool Manager::EnableBlend(bool bEnable)
		{
			m_bEnableBlend = bEnable;

			osg::Node* pData = m_p3DViewer->getSceneData();
			if (pData)
			{
				pData->getOrCreateStateSet()->setMode(GL_BLEND, m_bEnableBlend ? osg::StateAttribute::ON : osg::StateAttribute::OFF);
			}
			return m_bEnableBlend;
		}

		osg::Vec4 Manager::GetBackgroundColor3D()
		{
			try
			{
				nlohmann::json b3d = PFD::Config::IProgramConfig::GetInstance()->GetJson("BackgroundColor3D");
				bool IsNormal = b3d["IsNormal"].get<bool>();
				double R = b3d["R"].get<double>();
				double G = b3d["G"].get<double>();
				double B = b3d["B"].get<double>();
				if (IsNormal)
				{
					return osg::Vec4(R, G, B, 1.0);
				}
				return osg::Vec4(R / 255.0, G / 255.0, B / 255.0, 1.0);
			}
			catch (const std::exception& ex)
			{
				logger->error(ex.what());
			}

			return osg::Vec4(0.466, 0.533, 0.6, 1.0);	// default LightSlateGray
		}

		std::string LoadShaderString(const std::string& path)
		{
			std::ifstream	fs(path);
			std::stringstream	ss;
			ss << fs.rdbuf();
			std::string source = ss.str();
			return source;
		}

		bool Manager::ConfigureShaders(osg::StateSet* stateSet)
		{
			if (!stateSet)
			{
				return false;
			}

			osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, LoadShaderString(PFD::Config::IProgramConfig::GetInstance()->GetString("VERTEX_SHADER")));
			osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, LoadShaderString(PFD::Config::IProgramConfig::GetInstance()->GetString("FRAGMENT_SHADER")));

			osg::Program* program = new osg::Program;
			program->addShader(vShader);
			program->addShader(fShader);
			program->addBindAttribLocation(TANGENT, TANGENT_INDEX);
			stateSet->setAttribute(program);

			stateSet->setMode(GL_DEPTH, m_bEnableDepthTest ? osg::StateAttribute::ON : osg::StateAttribute::OFF);
			stateSet->setMode(GL_BLEND, m_bEnableBlend ? osg::StateAttribute::ON : osg::StateAttribute::OFF);

			osg::Vec3f lightDir(0., 0.5, 1.);
			lightDir.normalize();
			stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));

			return true;
		}

		void Manager::InitViewer(osg::ref_ptr<osg::GraphicsContext> gc)
		{
			m_p3DViewer = new osgViewer::Viewer;
			osg::Camera* cam = m_p3DViewer->getCamera();
			cam->setProjectionMatrix(osg::Matrix::perspective(30., (double)gc->getTraits()->width / (double)gc->getTraits()->height, 1., 100.));
			cam->setClearColor(GetBackgroundColor3D());
			cam->setGraphicsContext(gc);
			cam->setViewport(new osg::Viewport(0, 0, gc->getTraits()->width, gc->getTraits()->height));
			cam->setDrawBuffer(GL_BACK);
			cam->setReadBuffer(GL_BACK);

			osgViewer::Viewer::Windows windows;
			m_p3DViewer->getWindows(windows);
			for (osgViewer::Viewer::Windows::iterator itr = windows.begin();
				itr != windows.end();
				++itr)
			{
				osg::State* s = (*itr)->getState();
				s->setUseModelViewAndProjectionUniforms(true);
				s->setUseVertexAttributeAliasing(true);
			}

			m_p3DViewer->setThreadingModel(m_p3DViewer->suggestBestThreadingModel());

			logger->info("init 3d viewer done");
		}

		void Manager::RenderThread()
		{
			logger->info("Begin Render");
			m_p3DViewer->run();
			logger->info("End Render");
		}
	}


}