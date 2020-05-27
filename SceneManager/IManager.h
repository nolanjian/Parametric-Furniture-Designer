#pragma once
#include "SceneManagerExport.h"
#include "STLIncluding.h"
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
				Texture = 1000,
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
		};
	}
}