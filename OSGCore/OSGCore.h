// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the OSGCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// OSGCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OSGCORE_EXPORTS
#define OSGCORE_API __declspec(dllexport)
#else
#define OSGCORE_API __declspec(dllimport)
#endif

#include <memory>
#include <thread>

#include <osg/GraphicsContext>
#include <osgViewer/Viewer>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>

namespace OSGCore
{
	class OSGCORE_API OSGAdapt
	{
	public:
		OSGAdapt();

		bool Render(HWND hwnd);
		void Destroy();
		void RenderThread();

		void ShowGrids(bool bEnable);
		bool IsShowGrids();

		bool s_bKeepRunning = false;

		osg::ref_ptr<osg::Group>	Get3DScene();
		void						Set3DScene(osg::ref_ptr<osg::Group> ptr3DScene);

		void Cale(const std::wstring& pStrVal);

	private:
		std::thread	m_renderThread;
		HWND m_hwnd = NULL;

		osg::ref_ptr<osgViewer::Viewer>	m_ptrViewer;
		osg::ref_ptr<osg::Group>	m_ptr3DScene = new osg::Group();
	};
}
