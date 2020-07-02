#include "OpenGLWidget.h"
#include <QInputEvent>
#include <QPointer>
#include <QPainter>

#include "GraphicsWin.h"

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
				pGraphicsWin->SetOpenGLWidget(this);
			}
			if (m_renderThread.joinable())
			{
				m_renderThread.join();
			}
		}

		void OpenGLWidget::Init()
		{
			osg::Camera* pCamera = m_pViewer->getCamera();
			pCamera->setGraphicsContext(m_pGraphicsWindow);
			pCamera->setProjectionMatrix(osg::Matrix::perspective(30., (double)width() / (double)height(), 1., 10000.));
			//pCamera->setClearColor(GetBackgroundColor3D());
			pCamera->setViewport(new osg::Viewport(0, 0, width(), height()));
			pCamera->setDrawBuffer(GL_BACK);
			pCamera->setReadBuffer(GL_BACK);
		}

		void OpenGLWidget::RunRender()
		{
			m_renderThread = std::thread(std::bind(&OpenGLWidget::RenderThreading, this));
		}

		void OpenGLWidget::RenderThreading()
		{
			m_pViewer->run();
		}

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

			QPainter painter(this);

			paintGL();


		}

		void OpenGLWidget::moveEvent(QMoveEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QPoint & pos = event->pos();
			m_pGraphicsWindow->resized(pos.x(), pos.y(), width() * m_nDevicePixelRatio, height() * m_nDevicePixelRatio);
			m_pGraphicsWindow->getEventQueue()->windowResize(pos.x(), pos.y(), width() * m_nDevicePixelRatio, height() * m_nDevicePixelRatio);
			//m_pGraphicsWindow->requestRedraw();
		}

		void OpenGLWidget::resizeEvent(QResizeEvent* event)
		{
			Q_ASSERT(m_pGraphicsWindow);
			Q_ASSERT(event);
			const QSize& size = event->size();
			m_pGraphicsWindow->resized(x(), y(), size.width() * m_nDevicePixelRatio, size.height() * m_nDevicePixelRatio);
			m_pGraphicsWindow->getEventQueue()->windowResize(x(), y(), size.width() * m_nDevicePixelRatio, size.height() * m_nDevicePixelRatio);
			m_pGraphicsWindow->requestRedraw();
		}

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
				{ Qt::MouseButton::RightButton, osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON},
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