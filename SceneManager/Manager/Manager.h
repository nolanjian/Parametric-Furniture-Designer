/*****************************************************************//**
 * \file   Manager.h
 * \brief  Scene Manager Implement
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/09
 * 
 *********************************************************************/
#pragma once

#include "IManager.h"
#include <Commom/OSGIncluding.h>
#include <Utils/Utils.h>
#include <spdlog/spdlog.h>
#include "../TopScene/TopScene.h"

namespace PFD
{
	namespace Scene
	{
		class Manager : public IManager
		{
		public:
			explicit Manager();
			virtual ~Manager();

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
			void InitViewer();
			void RenderThread();

		private:
			bool PhotorealisticShaders(osg::StateSet* stateSet);
			bool WireFramesShaders(osg::StateSet* stateSet);
			bool CartoonShaders(osg::StateSet* stateSet);
			bool BlackAndWhiteShaders(osg::StateSet* stateSet);

		protected:
			std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);

		private:
			RenderMode m_renderMode = RenderMode::Photorealistic;

			HWND m_hwnd = nullptr;
			std::thread	m_renderThread;
			osg::ref_ptr<osgViewer::Viewer>	m_p3DViewer;
			osg::ref_ptr<TopScene>			m_pTopScene;

			bool m_bEnableDepthTest = true;
			bool m_bEnableBlend = true;
		};
	}
}