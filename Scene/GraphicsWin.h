/*****************************************************************//**
 * \file   GraphicsWin.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/02
 * 
 *********************************************************************/
/**
 * \brief  
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/03
 * */

#pragma once

#include <osgViewer/Viewer>
#include "OpenGLWidget.h"

namespace PFD
{
	namespace Scene
	{
		class GraphicsWin : public osgViewer::GraphicsWindow
		{
		public:
			GraphicsWin();

            virtual ~GraphicsWin();

			void SetOpenGLWidget(OpenGLWidget* pOpenGLWidget);
			OpenGLWidget* GetOpenGLWidget();

            /** Return whether a valid and usable GraphicsContext has been created.*/
            virtual bool valid() const override;

            /** Realize the GraphicsContext implementation,
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual bool realizeImplementation() override;

            /** Return true if the graphics context has been realized, and is ready to use, implementation.
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual bool isRealizedImplementation() const override;

            /** Close the graphics context implementation.
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual void closeImplementation() override;

            /** Make this graphics context current implementation.
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual bool makeCurrentImplementation() override;

            /** Make this graphics context current with specified read context implementation.
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual bool makeContextCurrentImplementation(GraphicsContext* readContext) override;

            /** Release the graphics context.*/
            virtual bool releaseContextImplementation() override;

            /** Pure virtual, Bind the graphics context to associated texture implementation.
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual void bindPBufferToTextureImplementation(GLenum /*buffer*/) override;

            /** Swap the front and back buffers implementation.
              * Pure virtual - must be implemented by concrete implementations of GraphicsContext. */
            virtual void swapBuffersImplementation() override;

            void init(int x, int y, int width, int height);

		private:
            bool m_bRealized = false;
			OpenGLWidget* m_pOpenGLWidget = nullptr;
		};
	}	// namespace Scene
}	// namespace PFD