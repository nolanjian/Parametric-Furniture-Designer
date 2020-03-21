#pragma once
#include <windows.h>

#include "OSGIncluding.h"
#include "STLIncluding.h"

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
		void SetOnSelectObjectCallback(void(*ptr)(long long int)) { fnOnSelectObjectCallback = ptr; }
		void LoadScene(const std::string& path);
		void OpenSceneInNewWindow(const std::string& path);
	protected:
	private:
		bool s_bKeepRunning = false;
		std::thread	m_renderThread;
		HWND m_hwnd = NULL;
		osg::ref_ptr<osgViewer::Viewer>	m_ptrViewer;
		osg::ref_ptr<osg::Group>	m_ptr3DScene = new osg::Group();

		void(*fnOnSelectObjectCallback)(long long int) = nullptr;
	};
}