/*****************************************************************//**
 * \file   OpenGLWidget.cpp
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/03
 * 
 *********************************************************************/
#include "OpenGLWidget.h"
#include "GraphicsWin.h"

#include <Qt>
#include <QApplication>
#include <QInputEvent>
#include <QPointer>
#include <QPainter>
#include <QSurface>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QOpenGLFunctions>

#include <nlohmann/json.hpp>


#include <Config/IProgramConfig.h>
#include <GLTFHelper/Importer.h>

#include <Commom/ShadingPreDefine.h>

#include <osgGA/TrackballManipulator>


namespace PFD
{
	namespace Scene
	{
		OpenGLWidget::OpenGLWidget(QWidget* parent, Qt::WindowFlags f)
			: QOpenGLWidget(parent, f)
			, m_nDevicePixelRatio(devicePixelRatio())
			, m_pViewer(new osgViewer::Viewer())
			, m_pGraphicsWindow(new GraphicsWin())
		{
			setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);

			GraphicsWin* pGraphicsWin = dynamic_cast<GraphicsWin*>(m_pGraphicsWindow.get());
			if (pGraphicsWin)
			{
				pGraphicsWin->SetOpenGLWidget(this);
			}
		}

		OpenGLWidget::~OpenGLWidget()
		{
			GraphicsWin* pGraphicsWin = dynamic_cast<GraphicsWin*>(m_pGraphicsWindow.get());
			if (pGraphicsWin)
			{
				pGraphicsWin->close();
			}
		}

		void OpenGLWidget::InitCamera()
		{
			m_pViewer->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);
			m_pViewer->setCameraManipulator(new osgGA::TrackballManipulator());

			osg::Camera* pCamera = m_pViewer->getCamera();

			//pCamera->setRenderTargetImplementation(osg::Camera::RenderTargetImplementation(100));
			pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			pCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
			pCamera->setGraphicsContext(m_pGraphicsWindow);
			pCamera->setProjectionMatrix(osg::Matrix::perspective(30., (double)width() / (double)height(), 1., 100));
			pCamera->setClearColor(GetBackgroundColor3D());
			pCamera->setViewport(new osg::Viewport(x(), y(), width(), height()));
			pCamera->setDrawBuffer(GL_BACK);
			pCamera->setReadBuffer(GL_BACK);

			m_pGraphicsWindow->getEventQueue()->syncWindowRectangleWithGraphicsContext();
		}

		void OpenGLWidget::OpenFile()
		{
			//InitCamera();

			QString file_name = QFileDialog::getOpenFileName(NULL, "��GLTF�ļ�", ".", "*.gltf *.glb");
			std::wstring path = file_name.toStdWString();

			try
			{
				auto pImporter = PFD::GLTF::Importer::Create();
				if (!pImporter)
				{
					//logger->error("Create GLTF importer fail");
					//return false;
				}

				osg::ref_ptr<osg::Group> pScene = pImporter->Load(path);
				if (!pScene)
				{
					//logger->error("Load GLTF {} fail", PFD::Utils::WStringToString(path));
					//return false;
				}

				PhotorealisticShaders(pScene->getOrCreateStateSet());

				if (m_pViewer)
				{
					m_pViewer->setSceneData(pScene);
				}

				//paintGL();
			}
			catch (const std::exception& ex)
			{
				//logger->error(ex.what());
				//return false;
			}
		}

		void OpenGLWidget::BeginTimer()
		{
			m_timer = new QTimer(this);
			m_timer->connect(m_timer, &QTimer::timeout, [this]() {
				this->update();
				});
			m_timer->start(10);
		}

		osg::Vec4 OpenGLWidget::GetBackgroundColor3D()
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
				//logger->error(ex.what());
			}

			return osg::Vec4(0.466, 0.533, 0.6, 1.0);	// default LightSlateGray
		}

		void OpenGLWidget::initializeGL()
		{
			initializeOpenGLFunctions();

			makeCurrent();

			emit initialized();
		}

		void OpenGLWidget::resizeGL(int w, int h)
		{
			GLuint defaultFBO = defaultFramebufferObject();

			setDefaultDisplaySettings();

			InitCamera();

			Q_ASSERT(m_pGraphicsWindow);
			m_pGraphicsWindow->resized(x(), y(), w * m_nDevicePixelRatio, h * m_nDevicePixelRatio);
			m_pGraphicsWindow->getEventQueue()->windowResize(x(), y(), w * m_nDevicePixelRatio, h * m_nDevicePixelRatio);
			m_pGraphicsWindow->requestRedraw();
		}

		void OpenGLWidget::paintGL()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if (!bInitOSG)
			{
				

				GLint fboID = defaultFramebufferObject();
				if (fboID != 0)
				{
					GraphicsWin* pGraphicsWin = dynamic_cast<GraphicsWin*>(m_pGraphicsWindow.get());
					if (pGraphicsWin)
					{
						pGraphicsWin->init(x(), y(), width(), height());
						m_pViewer->getCamera()->getGraphicsContext()->setDefaultFboId(fboID);
					}
				}
				bInitOSG = true;

				BeginTimer();
			}

			if (bInitOSG)
			{
				m_pViewer->frame();
			}
		}

		//bool OpenGLWidget::event(QEvent* e)
		//{
		//	d->fbo;


		//	return false;
		//}

		void OpenGLWidget::mousePressEvent(QMouseEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint& pos = event->pos();
			const Qt::MouseButton& mouseButton = event->button();

			m_pGraphicsWindow->getEventQueue()->mouseButtonPress(pos.x(), pos.y(), MouseButtonMap::Get().GetKey(mouseButton));
		}

		void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint& pos = event->pos();
			const Qt::MouseButton& mouseButton = event->button();

			m_pGraphicsWindow->getEventQueue()->mouseButtonRelease(pos.x(), pos.y(), MouseButtonMap::Get().GetKey(mouseButton));
		}

		void OpenGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint& pos = event->pos();
			const Qt::MouseButton& mouseButton = event->button();

			m_pGraphicsWindow->getEventQueue()->mouseDoubleButtonPress(pos.x(), pos.y(), MouseButtonMap::Get().GetKey(mouseButton));
		}

		void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint& pos = event->pos();
			const Qt::MouseButton& mouseButton = event->button();

			m_pGraphicsWindow->getEventQueue()->mouseMotion(pos.x(), pos.y(), MouseButtonMap::Get().GetKey(mouseButton));
		}

		void OpenGLWidget::wheelEvent(QWheelEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint& pos = event->pos();
			auto sm = event->delta() > 0 ?
				osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP : osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN;

			m_pGraphicsWindow->getEventQueue()->mouseScroll(sm);
		}

		void OpenGLWidget::keyPressEvent(QKeyEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);

			m_pGraphicsWindow->getEventQueue()->keyPress(KeyMap::Get().GetKey(Qt::Key(event->key())));
		}

		void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);

			m_pGraphicsWindow->getEventQueue()->keyRelease(KeyMap::Get().GetKey(Qt::Key(event->key())));
		}

		void OpenGLWidget::focusInEvent(QFocusEvent* event)
		{
		}

		void OpenGLWidget::focusOutEvent(QFocusEvent* event)
		{
		}

		void OpenGLWidget::enterEvent(QEvent* event)
		{
		}

		void OpenGLWidget::leaveEvent(QEvent* event)
		{
		}

		void OpenGLWidget::paintEvent(QPaintEvent* event)
		{
			makeCurrent();

			//QPainter painter(this);

			paintGL();

//#ifdef WITH_SELECTION_PROCESSING
//			if (selectionActive_ && !selectionFinished_)
//			{
//				painter.setPen(Qt::black);
//				painter.setBrush(Qt::transparent);
//				painter.drawRect(makeRectangle(selectionStart_, selectionEnd_));
//			}
//#endif

			//painter.end();

			doneCurrent();
		}

		void OpenGLWidget::moveEvent(QMoveEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint& pos = event->pos();
			m_pGraphicsWindow->resized(pos.x(), pos.y(), width() * m_nDevicePixelRatio, height() * m_nDevicePixelRatio);
			m_pGraphicsWindow->getEventQueue()->windowResize(pos.x(), pos.y(), width() * m_nDevicePixelRatio, height() * m_nDevicePixelRatio);
			//m_pGraphicsWindow->requestRedraw();
		}

		//void OpenGLWidget::resizeEvent(QResizeEvent* event)
		//{
		//	Q_ASSERT(m_pGraphicsWindow);
		//	Q_ASSERT(event);
		//	const QSize& size = event->size();
		//	m_pGraphicsWindow->resized(x(), y(), size.width() * m_nDevicePixelRatio, size.height() * m_nDevicePixelRatio);
		//	m_pGraphicsWindow->getEventQueue()->windowResize(x(), y(), size.width() * m_nDevicePixelRatio, size.height() * m_nDevicePixelRatio);
		//	m_pGraphicsWindow->requestRedraw();
		//}

		void OpenGLWidget::closeEvent(QCloseEvent* event)
		{
		}

		void OpenGLWidget::contextMenuEvent(QContextMenuEvent* event)
		{
		}

		void OpenGLWidget::tabletEvent(QTabletEvent* event)
		{
		}

		void OpenGLWidget::actionEvent(QActionEvent* event)
		{
		}

		void OpenGLWidget::dragEnterEvent(QDragEnterEvent* event)
		{
		}

		void OpenGLWidget::dragMoveEvent(QDragMoveEvent* event)
		{
		}

		void OpenGLWidget::dragLeaveEvent(QDragLeaveEvent* event)
		{
		}

		void OpenGLWidget::dropEvent(QDropEvent* event)
		{
		}

		void OpenGLWidget::showEvent(QShowEvent* event)
		{
		}

		void OpenGLWidget::hideEvent(QHideEvent* event)
		{
		}

		bool OpenGLWidget::PhotorealisticShaders(osg::StateSet* stateSet)
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

			stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
			stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

			osg::Vec3f lightDir(0., 0.5, 1.);
			lightDir.normalize();
			stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));

			return true;
		}

		void OpenGLWidget::setDefaultDisplaySettings()
		{
			osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
			ds->setNvOptimusEnablement(1);
			ds->setStereo(false);
		}

		MouseButtonMap& MouseButtonMap::Get()
		{
			static MouseButtonMap s_MouseButtonMap;
			return s_MouseButtonMap;
		}

		int MouseButtonMap::GetKey(Qt::MouseButton mouseButton)
		{
			if (m_Mapping.find(mouseButton) != m_Mapping.end())
			{
				return m_Mapping[mouseButton];
			}
			return 0;
		}

		MouseButtonMap::MouseButtonMap()
		{
			m_Mapping = std::move(std::map<Qt::MouseButton, osgGA::GUIEventAdapter::MouseButtonMask>{
				{ Qt::MouseButton::LeftButton, osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON },
				{ Qt::MouseButton::RightButton, osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON },
				{ Qt::MouseButton::MiddleButton, osgGA::GUIEventAdapter::MouseButtonMask::MIDDLE_MOUSE_BUTTON },
			});
		}

		KeyMap& KeyMap::Get()
		{
			static KeyMap s_KeyMap;
			return s_KeyMap;
		}

		int KeyMap::GetKey(Qt::Key key)
		{
			if (m_Mapping.find(key) != m_Mapping.end())
			{
				return m_Mapping[key];
			}
			return 0;
		}

		KeyMap::KeyMap()
		{
			m_Mapping = std::move(std::map<Qt::Key, osgGA::GUIEventAdapter::KeySymbol> {
				{ Qt::Key::Key_Space, osgGA::GUIEventAdapter::KeySymbol::KEY_Space },
			});
		}

	}	// namespace Scene
}	// namespace PFD