#pragma once

#include "wx/defs.h"
#include "wx/control.h"
#include "wx/timer.h"
#include "wke.h"

namespace PFD
{
	namespace GUI
	{
		class WebView : public wxControl
		{
		public:
			explicit WebView(wxWindow* parent,
				wxWindowID id = wxID_ANY,
				const wxString& url = "",
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = 0,
				const wxString& name = wxControlNameStr);
			virtual ~WebView();

			void ShowDevTool(const wxString& path);

		// callbacks
		protected:
			void InitCallback();

			static void wkeTitleChangedCallback(wkeWebView webView, void* param, const wkeString title);
			static void wkeURLChangedCallback(wkeWebView webView, void* param, const wkeString url);
			static void wkeURLChangedCallback2(wkeWebView webView, void* param, wkeWebFrameHandle frameId, const wkeString url);
			static void wkePaintUpdatedCallback(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy);
			static void wkePaintBitUpdatedCallback(wkeWebView webView, void* param, const void* buffer, const wkeRect* r, int width, int height);
			static void wkeAlertBoxCallback(wkeWebView webView, void* param, const wkeString msg);
			static bool wkeConfirmBoxCallback(wkeWebView webView, void* param, const wkeString msg);
			static bool wkePromptBoxCallback(wkeWebView webView, void* param, const wkeString msg, const wkeString defaultResult, wkeString result);
			static bool wkeNavigationCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url);
			static wkeWebView wkeCreateViewCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);
			static void wkeDocumentReadyCallback(wkeWebView webView, void* param);
			static void wkeDocumentReady2Callback(wkeWebView webView, void* param, wkeWebFrameHandle frameId);
			static void wkeLoadingFinishCallback(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason);
			static bool wkeDownloadCallback(wkeWebView webView, void* param, const char* url);
			static wkeDownloadOpt wkeDownload2Callback(
				wkeWebView webView,
				void* param,
				size_t expectedContentLength,
				const char* url,
				const char* mime,
				const char* disposition,
				wkeNetJob job,
				wkeNetJobDataBind* dataBind);
			static void wkeConsoleCallback(wkeWebView webView, void* param, wkeConsoleLevel level, const wkeString message, const wkeString sourceName, unsigned sourceLine, const wkeString stackTrace);
			static void wkeOnCallUiThread(wkeWebView webView, void* paramOnInThread);
			static void wkeCallUiThread(wkeWebView webView, void(*func)(wkeWebView, void*), void* param);
			static wkeMediaPlayer  wkeMediaPlayerFactory(wkeWebView webView, wkeMediaPlayerClient client, void* npBrowserFuncs, void* npPluginFuncs);
			static bool  wkeOnIsMediaPlayerSupportsMIMEType(const utf8* mime);
			static bool wkeLoadUrlBeginCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job);
			static void wkeLoadUrlEndCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job, void* buf, int len);
			static void  wkeLoadUrlFailCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job);
			static void wkeDidCreateScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int extensionGroup, int worldId);
			static void wkeWillReleaseScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int worldId);
			static bool wkeNetResponseCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job);
			static void wkeOnNetGetFaviconCallback(wkeWebView webView, void* param, const utf8* url, wkeMemBuf* buf);
			static bool wkeWindowClosingCallback(wkeWebView webWindow, void* param);
			static void wkeWindowDestroyCallback(wkeWebView webWindow, void* param);
			static void wkeDraggableRegionsChangedCallback(wkeWebView webView, void* param, const wkeDraggableRegion* rects, int rectCount);
			static void wkeWillMediaLoadCallback(wkeWebView webView, void* param, const char* url, wkeMediaLoadInfo* info);
			static void wkeStartDraggingCallback(
				wkeWebView webView,
				void* param,
				wkeWebFrameHandle frame,
				const wkeWebDragData* data,
				wkeWebDragOperationsMask mask,
				const void* image,
				const wkePoint* dragImageOffset
			);
			static void wkeOnPrintCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* printParams);
			static void wkeOnOtherLoadCallback(wkeWebView webView, void* param, wkeOtherLoadType type, wkeTempCallbackInfo* info);
			static bool wkeOnContextMenuItemClickCallback(
				wkeWebView webView,
				void* param,
				wkeOnContextMenuItemClickType type,
				wkeOnContextMenuItemClickStep step,
				wkeWebFrameHandle frameId,
				void* info
				);
			static void wkeOnPluginFindCallback(wkeWebView webView, void* param, const utf8* mime, void* initializeFunc, void* getEntryPointsFunc, void* shutdownFunc);
			static void wkeOnShowDevtoolsCallback(wkeWebView webView, void* param);
		private:
			wxString m_url;
			wkeWebView	m_wkeView = nullptr;
		};	/// class WebView
	}	/// namespace GUI
}	/// namespace PFD