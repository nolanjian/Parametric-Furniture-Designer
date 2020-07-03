/*****************************************************************//**
 * \file   OpenGLWidget.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/02
 * 
 *********************************************************************/
#pragma once

#include "scene_global.h"
#include <QOpenGLWidget>

#include <osgViewer/Viewer>

#include <thread>
#include <map>

namespace PFD
{
	namespace Scene
	{
		class MouseButtonMap;
		class KeyMap;

		class SCENE_EXPORT OpenGLWidget : public QOpenGLWidget
		{
			Q_OBJECT
		public:
			explicit OpenGLWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
			~OpenGLWidget();

			void InitCamera();
		//signals:
		//	void triggered(bool checked = false);

		public slots:
			void OpenFile();

		protected:
			osg::Vec4 GetBackgroundColor3D();

		protected:
			virtual void initializeGL() override;
			virtual void resizeGL(int w, int h) override;
			virtual void paintGL() override;

		protected:
			// Event handlers
			//bool event(QEvent* event) override;
			virtual void mousePressEvent(QMouseEvent* event) override;
			virtual void mouseReleaseEvent(QMouseEvent* event) override;
			virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
			virtual void mouseMoveEvent(QMouseEvent* event) override;
#if QT_CONFIG(wheelevent)
			virtual void wheelEvent(QWheelEvent* event) override;
#endif
			virtual void keyPressEvent(QKeyEvent* event) override;
			virtual void keyReleaseEvent(QKeyEvent* event) override;
			virtual void focusInEvent(QFocusEvent* event) override;
			virtual void focusOutEvent(QFocusEvent* event) override;
			virtual void enterEvent(QEvent* event) override;
			virtual void leaveEvent(QEvent* event) override;
			virtual void paintEvent(QPaintEvent* event) override;
			virtual void moveEvent(QMoveEvent* event) override;
			virtual void resizeEvent(QResizeEvent* event) override;
			virtual void closeEvent(QCloseEvent* event) override;
#ifndef QT_NO_CONTEXTMENU
			virtual void contextMenuEvent(QContextMenuEvent* event) override;
#endif
#if QT_CONFIG(tabletevent)
			virtual void tabletEvent(QTabletEvent* event) override;
#endif
#ifndef QT_NO_ACTION
			virtual void actionEvent(QActionEvent* event) override;
#endif

#if QT_CONFIG(draganddrop)
			virtual void dragEnterEvent(QDragEnterEvent* event) override;
			virtual void dragMoveEvent(QDragMoveEvent* event) override;
			virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
			virtual void dropEvent(QDropEvent* event);
#endif

			virtual void showEvent(QShowEvent* event) override;
			virtual void hideEvent(QHideEvent* event) override;

//#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
//			virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
//#else
//			virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);
//#endif
		private:
			bool PhotorealisticShaders(osg::StateSet* stateSet);

		private:
			osg::ref_ptr<osgViewer::Viewer> m_pViewer;
			osg::ref_ptr<osgViewer::GraphicsWindow> m_pGraphicsWindow;
			int m_nDevicePixelRatio = 1;

			QTimer* m_timer;
		};

		class MouseButtonMap
		{
		public:
			static MouseButtonMap& Get();

			int GetKey(Qt::MouseButton mouseButton);

		protected:
			MouseButtonMap();
		private:
			MouseButtonMap(const MouseButtonMap&) = delete;
			MouseButtonMap(const MouseButtonMap&&) = delete;
			MouseButtonMap& operator= (const MouseButtonMap&) = delete;
			MouseButtonMap& operator= (const MouseButtonMap&&) = delete;

			std::map<Qt::MouseButton, osgGA::GUIEventAdapter::MouseButtonMask>	m_Mapping;
		};

		class KeyMap
		{
		public:
			static KeyMap& Get();

			int GetKey(Qt::Key key);

		protected:
			KeyMap();
		private:
			KeyMap(const KeyMap&) = delete;
			KeyMap(const KeyMap&&) = delete;
			KeyMap& operator= (const KeyMap&) = delete;
			KeyMap& operator= (const KeyMap&&) = delete;

			std::map<Qt::Key, osgGA::GUIEventAdapter::KeySymbol>	m_Mapping;
		};

	}	// namespace Scene
}	// namespace PFD