#pragma once
#include <windows.h>
#include <thread>
#include <osgViewer/Viewer>
#include <osg/Group>
#include <string>

namespace OSGCore
{
	class ImplementOSGCore
	{
	public:
		ImplementOSGCore();
		virtual ~ImplementOSGCore();

		bool Render(HWND);
		void Destory();
		void RenderThread();
		void SetModelPath(const std::wstring& path);
	protected:
	private:
		bool s_bKeepRunning = false;
		std::thread	m_renderThread;
		HWND m_hwnd = NULL;
		osg::ref_ptr<osgViewer::Viewer>	m_ptrViewer;
		osg::ref_ptr<osg::Group>	m_ptr3DScene = new osg::Group();
	};
}