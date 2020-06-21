/*****************************************************************//**
 * \file   IManager.h
 * \brief  Scene Manager for 3D window
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/09
 * 
 *********************************************************************/
#pragma once
#include "../SceneManagerExport.h"
#include <Commom/STLIncluding.h>
#include "windows.h"

namespace PFD
{
	namespace Scene
	{
		class PFD_SCENEMANAGER_API IManager
		{
		public:
			enum RenderMode
			{
				Photorealistic = 1000,
				WireFrames,
				Cartoon,
				BlackAndWhite,
			};

			virtual ~IManager() {}
			static IManager* GetInstance();

			// window hwnd
			virtual void SetHWND(HWND hwnd) = 0;
			virtual HWND GetHWND() = 0;

			virtual bool BeginRender() = 0;
			virtual bool StopRender() = 0;

			virtual void SetRenderMode(RenderMode mode) = 0;
			virtual RenderMode GetRenderMode() = 0;

			virtual bool ImportScene(const std::wstring& path) = 0;
			virtual bool ImportScene(const std::string& utf8Json) = 0;
			virtual bool ExportScene(const std::wstring& path) = 0;
			virtual bool ExportScene(std::string& utf8Json) = 0;
			virtual bool CloseScene() = 0;

			/// Some OpenGL Options
			virtual bool EnableDepthTest(bool bEnable = true) = 0;
			virtual bool EnableBlend(bool bEnable = true) = 0;
		
		protected:
			IManager() {}
			IManager(const IManager&) = delete;
			IManager(const IManager&&) = delete;
			IManager& operator= (const IManager&) = delete;
			IManager& operator= (const IManager&&) = delete;
		};
	}
}