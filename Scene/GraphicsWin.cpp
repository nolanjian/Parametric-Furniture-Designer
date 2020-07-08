/*****************************************************************//**
 * \file   GraphicsWin.cpp
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/03
 * 
 *********************************************************************/

#include "GraphicsWin.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace PFD
{
	namespace Scene
	{
		GraphicsWin::GraphicsWin()
			: osgViewer::GraphicsWindow()
		{
			osg::setNotifyLevel(osg::NotifySeverity::NOTICE);
			osg::setNotifyHandler(new osg::WinDebugNotifyHandler());
		}

		GraphicsWin::~GraphicsWin()
		{
		}

		void GraphicsWin::SetOpenGLWidget(OpenGLWidget* pOpenGLWidget)
		{
			m_pOpenGLWidget = pOpenGLWidget;
		}

		OpenGLWidget* GraphicsWin::GetOpenGLWidget()
		{
			return m_pOpenGLWidget;
		}

		bool GraphicsWin::valid() const
		{
			return m_pOpenGLWidget && m_pOpenGLWidget->isValid();
		}

		bool GraphicsWin::realizeImplementation()
		{
			assert(m_pOpenGLWidget);

			QOpenGLContext* pPreQOpenGLContext = QOpenGLContext::currentContext();
			if (!valid() && pPreQOpenGLContext)
			{
				pPreQOpenGLContext->makeCurrent(pPreQOpenGLContext->surface());
				return false;
			}

			makeCurrentImplementation();

			getEventQueue()->syncWindowRectangleWithGraphicsContext();

			releaseContextImplementation();

			m_bRealized = true;

			if (pPreQOpenGLContext)
			{
				return pPreQOpenGLContext->makeCurrent(pPreQOpenGLContext->surface());
			}

			return true;
		}

		bool GraphicsWin::isRealizedImplementation() const
		{
			return m_bRealized;
		}

		void GraphicsWin::closeImplementation()
		{
			assert(m_pOpenGLWidget);
			m_pOpenGLWidget->close();
		}

		bool GraphicsWin::makeCurrentImplementation()
		{
			assert(m_pOpenGLWidget);

			if (QOpenGLContext::currentContext() != m_pOpenGLWidget->context())
			{
				m_pOpenGLWidget->makeCurrent();
			}

			return true;
		}

		bool GraphicsWin::makeContextCurrentImplementation(GraphicsContext* readContext)
		{
			// not implement
			return false;
		}

		bool GraphicsWin::releaseContextImplementation()
		{
			assert(m_pOpenGLWidget);
			m_pOpenGLWidget->doneCurrent();
			return true;
		}

		void GraphicsWin::bindPBufferToTextureImplementation(GLenum)
		{
			// not implement
		}

		void GraphicsWin::swapBuffersImplementation()
		{
			assert(m_pOpenGLWidget);
			
			if (QOpenGLContext::currentContext() != m_pOpenGLWidget->context())
			{
				m_pOpenGLWidget->makeCurrent();
			}

			QOpenGLContext* pQOpenGLContext = m_pOpenGLWidget->context();
			if (pQOpenGLContext)
			{

				pQOpenGLContext->swapBuffers(pQOpenGLContext->surface());
			}

			m_pOpenGLWidget->frameSwapped();

		}

		void GraphicsWin::init(int x, int y, int width, int height)
		{
			_traits = new osg::GraphicsContext::Traits();
			_traits->x = x;
			_traits->y = y;
			_traits->width = width;
			_traits->height = height;
			_traits->doubleBuffer = true;
			_traits->windowDecoration = true;

			;

			qglFormat2traits(m_pOpenGLWidget->format(), *_traits);

			if (valid())
			{
				setState(new osg::State);
				getState()->setCheckForGLErrors(osg::State::ONCE_PER_ATTRIBUTE);
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

		void GraphicsWin::qglFormat2traits(const QSurfaceFormat& qSurfaceFormat, osg::GraphicsContext::Traits& osgTraits)
		{
			osgTraits.red = qSurfaceFormat.redBufferSize();
			osgTraits.green = qSurfaceFormat.greenBufferSize();
			osgTraits.blue = qSurfaceFormat.blueBufferSize();
			osgTraits.alpha = qSurfaceFormat.hasAlpha() ? qSurfaceFormat.alphaBufferSize() : 0;
			osgTraits.depth = qSurfaceFormat.depthBufferSize();
			osgTraits.stencil = qSurfaceFormat.stencilBufferSize();

			osgTraits.samples = qSurfaceFormat.samples();
			osgTraits.quadBufferStereo = qSurfaceFormat.stereo();
			osgTraits.doubleBuffer = qSurfaceFormat.swapBehavior() == QSurfaceFormat::DoubleBuffer;
			osgTraits.vsync = qSurfaceFormat.swapInterval() >= 1;
		}

	}	// namespace Scene
}	// namespace PFD