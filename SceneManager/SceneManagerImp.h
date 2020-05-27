#pragma once

#include "ISceneManager.h"
#include "windows.h"
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "Utils.h"
#include "string.h"

namespace PFDSceneManager
{
	class SceneManagerImp : public ISceneManager
	{
	public:
		explicit SceneManagerImp();
		virtual ~SceneManagerImp();

		SceneManagerImp(const SceneManagerImp&) = delete;
		SceneManagerImp(const SceneManagerImp&&) = delete;
		SceneManagerImp& operator= (const SceneManagerImp&) = delete;
		SceneManagerImp& operator= (const SceneManagerImp&&) = delete;

		

		virtual void SetHWND(HWND hwnd) override;
		virtual HWND GetHWND() override;

		virtual bool BeginRender() override;
		virtual bool StopRender() override;

		virtual void SetRenderMode(RenderMode mode) override;
		virtual RenderMode GetRenderMode() override;

		virtual bool ImportScene(const std::wstring& path) override;
		virtual bool ImportScene(const std::string& utf8Json) override;
		virtual bool ExportScene(const std::wstring& path) override;
		virtual bool ExportScene(std::string& utf8Json) override;

		virtual bool CloseScene() override;
		
		/// Some OpenGL Options
		virtual bool EnableDepthTest(bool bEnable = true) override;
		virtual bool EnableBlend(bool bEnable = true) override;

	protected:
		bool OpenScene(osg::ref_ptr<osg::Group> pScene);
		osg::Vec4 GetBackgroundColor3D();
		bool ConfigureShaders(osg::StateSet* stateSet);
		void InitViewer(osg::ref_ptr<osg::GraphicsContext> gc);
		void RenderThread();

	protected:
		std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);

	private:
		RenderMode m_renderMode = RenderMode::Texture;

		HWND m_hwnd = nullptr;
		std::thread	m_renderThread;
		osg::ref_ptr<osgViewer::Viewer>	m_p3DViewer;

		bool m_bEnableDepthTest = true;
		bool m_bEnableBlend = true;
	};
}