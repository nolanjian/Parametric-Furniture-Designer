#include "GraphicsWin.h"
#include <QOpenGLContext>

namespace PFD
{
	namespace Scene
	{
		GraphicsWin::GraphicsWin()
			: osgViewer::GraphicsWindow()
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
			//return m_bRealized;
			return true;
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
			QOpenGLContext* pQOpenGLContext = m_pOpenGLWidget->context();
			if (pQOpenGLContext)
			{
				pQOpenGLContext->swapBuffers(pQOpenGLContext->surface());
			}

			if (QOpenGLContext::currentContext() != m_pOpenGLWidget->context())
			{
				m_pOpenGLWidget->makeCurrent();
			}
		}

		void GraphicsWin::init(int x, int y, int width, int height)
		{
			_traits = new osg::GraphicsContext::Traits();
			_traits->x = x;
			_traits->y = y;
			_traits->width = width;
			_traits->height = height;

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

	}	// namespace Scene
}	// namespace PFD