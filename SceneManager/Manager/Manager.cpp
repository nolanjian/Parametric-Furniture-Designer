#include "Manager.h"
#include <Config/IProgramConfig.h>
#include <GLTFHelper/Importer.h>
#include <GLTFHelper/Exporter.h>
#include <Commom/ShadingPreDefine.h>

namespace PFD
{
	namespace Scene
	{
		Manager::Manager()
		{
			osg::setNotifyLevel(osg::NOTICE);
			osg::setNotifyHandler(new osg::WinDebugNotifyHandler());
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
			try
			{
				if (!StopRender())
				{
					throw std::exception("Can`t Stop Render before");
				}
				InitViewer();
				m_renderThread = std::thread(std::bind(&Manager::RenderThread, this));
				return true;
			}
			catch (const std::exception& ex)
			{
				logger->error(ex.what());
				return false;
			}
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
			if (m_renderMode == mode)
			{
				return;
			}

			switch (mode)
			{
			case IManager::Photorealistic:
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
				auto pImporter = PFD::GLTF::Importer::Create();
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
			auto pImporter = PFD::GLTF::Importer::Create();
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
			auto pExporter = PFD::GLTF::Exporter::Create();
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
			auto pExporter = PFD::GLTF::Exporter::Create();
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
			if (m_p3DViewer && m_pTopScene)
			{
				CloseScene();
				ConfigureShaders(pScene->getOrCreateStateSet());
				//m_p3DViewer->setSceneData(pScene);
				m_p3DViewer->setSceneData(m_pTopScene);
				m_pTopScene->AddObject(pScene);
				return true;
			}
			return false;
		}

		bool Manager::CloseScene()
		{
			if (m_p3DViewer && m_pTopScene)
			{
				m_pTopScene->CleanObjects();
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

		bool Manager::ConfigureShaders(osg::StateSet* stateSet)
		{
			if (!stateSet)
			{
				return false;
			}

			switch (m_renderMode)
			{
			case PFD::Scene::IManager::Photorealistic:
				return PhotorealisticShaders(stateSet);
			case PFD::Scene::IManager::WireFrames:
				return WireFramesShaders(stateSet);
			case PFD::Scene::IManager::Cartoon:
				return CartoonShaders(stateSet);
			case PFD::Scene::IManager::BlackAndWhite:
				return BlackAndWhiteShaders(stateSet);
			default:
				return false;
			}
		}

		void Manager::InitViewer()
		{
			if (m_hwnd == NULL)
			{
				throw std::exception("hwnd is null");
			}

			osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(nullptr);
			traits->inheritedWindowData = new osgViewer::GraphicsWindowWin32::WindowData(m_hwnd);
			traits->doubleBuffer = true;
			osg::GraphicsContext* pGraphicsContext = osg::GraphicsContext::createGraphicsContext(traits.get());
			if (!pGraphicsContext)
			{
				throw std::exception("GraphicsContext fail");
			}

			int nWidth = traits->width;
			int nHeight = traits->height;
			m_p3DViewer = new osgViewer::Viewer;
			
			osg::Camera* pCamera = m_p3DViewer->getCamera();
			pCamera->setGraphicsContext(pGraphicsContext);
			pCamera->setProjectionMatrix(osg::Matrix::perspective(30., (double)nWidth / (double)nHeight, 1., 100.));
			pCamera->setClearColor(GetBackgroundColor3D());
			pCamera->setViewport(new osg::Viewport(0, 0, nWidth, nHeight));
			pCamera->setDrawBuffer(GL_BACK);
			pCamera->setReadBuffer(GL_BACK);

#ifdef OSG_GL_FIXED_FUNCTION_AVAILABLE
			/**
			 * According to compile Options, these code no need to be built
			 * OSG_GL_FIXED_FUNCTION_AVAILABLE is not allow.
			 */
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
#endif // OSG_GL_FIXED_FUNCTION_AVAILABLE

			//m_p3DViewer->setThreadingModel(m_p3DViewer->suggestBestThreadingModel());

			m_pTopScene = new TopScene();
			m_pTopScene->ReInit();

			m_p3DViewer->setSceneData(m_pTopScene);

			logger->info("init 3d viewer done");
			return;
		}

		void Manager::RenderThread()
		{
			logger->info("Begin Render");
			m_p3DViewer->run();
			logger->info("End Render");
		}

		bool Manager::PhotorealisticShaders(osg::StateSet* stateSet)
		{
			std::string strVS = PFD::Utils::LoadStringFromFile(PFD::Config::IProgramConfig::GetInstance()->GetString("VERTEX_SHADER"));
			std::string strFS = PFD::Utils::LoadStringFromFile(PFD::Config::IProgramConfig::GetInstance()->GetString("FRAGMENT_SHADER"));

			osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, strVS);
			osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, strFS);

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

		bool Manager::WireFramesShaders(osg::StateSet* stateSet)
		{
			throw std::exception("WireFramesShaders not Implement");
		}

		bool Manager::CartoonShaders(osg::StateSet* stateSet)
		{
			throw std::exception("CartoonShaders not Implement");
		}

		bool Manager::BlackAndWhiteShaders(osg::StateSet* stateSet)
		{
			throw std::exception("BlackAndWhiteShaders not Implement");
		}

	}
}