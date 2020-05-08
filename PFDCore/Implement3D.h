#pragma once
#include <windows.h>

#include "OSGIncluding.h"
#include "STLIncluding.h"

namespace PFDCore
{
	class Implement3D
	{
	public:
		Implement3D();
		virtual ~Implement3D();

		bool Render(HWND);
		void Destory();
		void RenderThread();
		void SetOnSelectObjectCallback(void(*ptr)(long long int)) { fnOnSelectObjectCallback = ptr; }
		void LoadScene(const std::string& path);
		void CloseScene();
		void OpenSceneInNewWindow(const std::string& path);

		void configureShaders(osg::StateSet* stateSet);
		void initViewer(osg::ref_ptr<osg::GraphicsContext> gc);

	protected:
		osg::Vec4	GetBackgroundColor3D();

	private:
		std::thread	m_renderThread;
		HWND m_hwnd = NULL;
		osg::ref_ptr<osgViewer::Viewer>	m_ptrViewer;
		osg::ref_ptr<osg::Group>	m_ptr3DScene = new osg::Group();

		void(*fnOnSelectObjectCallback)(long long int) = nullptr;
	};
}