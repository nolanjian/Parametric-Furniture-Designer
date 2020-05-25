#include "PFDWebView.h"

namespace PFD
{
	namespace GUI
	{
		WebView::WebView(wxWindow* parent, wxWindowID id, const wxString& url, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
			: wxControl(parent, id, pos, size, style | wxNO_BORDER | wxCLIP_CHILDREN, wxDefaultValidator, name)
			, m_url(url)
		{
			if (wkeIsInitialize() == FALSE)
				wkeInitialize();

			wxSize clientSize = GetClientSize();
			m_wkeView = wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, GetHWND(), 0, 0, clientSize.GetWidth(), clientSize.GetHeight());
			InitCallback();
		}
		WebView::~WebView()
		{
			if (m_wkeView)
				wkeDestroyWebWindow(m_wkeView);
		}
		void WebView::ShowDevTool(const wxString& path)
		{
			wkeSetDebugConfig(m_wkeView, "showDevTools", path.ToUTF8());
		}
		void WebView::InitCallback()
		{
			if (m_wkeView == nullptr)
			{
				// TODO LOG
				return;
			}

			wkeOnMouseOverUrlChanged(m_wkeView, &WebView::wkeTitleChangedCallback, this);
			wkeOnTitleChanged(m_wkeView, &WebView::wkeTitleChangedCallback, this);
			wkeOnURLChanged(m_wkeView, &WebView::wkeURLChangedCallback, this);
			wkeOnURLChanged2(m_wkeView, &WebView::wkeURLChangedCallback2, this);
			wkeOnPaintUpdated(m_wkeView, &WebView::wkePaintUpdatedCallback, this);
			wkeOnPaintBitUpdated(m_wkeView, &WebView::wkePaintBitUpdatedCallback, this);
			wkeOnAlertBox(m_wkeView, &WebView::wkeAlertBoxCallback, this);
			wkeOnConfirmBox(m_wkeView, &WebView::wkeConfirmBoxCallback, this);
			wkeOnPromptBox(m_wkeView, &WebView::wkePromptBoxCallback, this);
			wkeOnNavigation(m_wkeView, &WebView::wkeNavigationCallback, this);
			wkeOnCreateView(m_wkeView, &WebView::wkeCreateViewCallback, this);
			wkeOnDocumentReady(m_wkeView, &WebView::wkeDocumentReadyCallback, this);
			wkeOnDocumentReady2(m_wkeView, &WebView::wkeDocumentReady2Callback, this);
			wkeOnLoadingFinish(m_wkeView, &WebView::wkeLoadingFinishCallback, this);
			wkeOnDownload(m_wkeView, &WebView::wkeDownloadCallback, this);
			wkeOnDownload2(m_wkeView, &WebView::wkeDownload2Callback, this);
			wkeOnConsole(m_wkeView, &WebView::wkeConsoleCallback, this);
			wkeSetUIThreadCallback(m_wkeView, &WebView::wkeCallUiThread, this);
			wkeOnLoadUrlBegin(m_wkeView, &WebView::wkeLoadUrlBeginCallback, this);
			wkeOnLoadUrlEnd(m_wkeView, &WebView::wkeLoadUrlEndCallback, this);
			wkeOnLoadUrlFail(m_wkeView, &WebView::wkeLoadUrlFailCallback, this);
			wkeOnDidCreateScriptContext(m_wkeView, &WebView::wkeDidCreateScriptContextCallback, this);
			wkeOnWillReleaseScriptContext(m_wkeView, &WebView::wkeWillReleaseScriptContextCallback, this);
			wkeOnWindowClosing(m_wkeView, &WebView::wkeWindowClosingCallback, this);
			wkeOnWindowDestroy(m_wkeView, &WebView::wkeWindowDestroyCallback, this);
			wkeOnDraggableRegionsChanged(m_wkeView, &WebView::wkeDraggableRegionsChangedCallback, this);
			wkeOnWillMediaLoad(m_wkeView, &WebView::wkeWillMediaLoadCallback, this);
			wkeOnStartDragging(m_wkeView, &WebView::wkeStartDraggingCallback, this);
			wkeOnPrint(m_wkeView, &WebView::wkeOnPrintCallback, this);
			wkeOnOtherLoad(m_wkeView, &WebView::wkeOnOtherLoadCallback, this);
			wkeOnContextMenuItemClick(m_wkeView, &WebView::wkeOnContextMenuItemClickCallback, this);
			//wkeOnPluginFind(m_wkeView, &WebView::wkeOnPluginFindCallback, this);
		}
		void WebView::wkeTitleChangedCallback(wkeWebView webView, void* param, const wkeString title)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeURLChangedCallback(wkeWebView webView, void* param, const wkeString url)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeURLChangedCallback2(wkeWebView webView, void* param, wkeWebFrameHandle frameId, const wkeString url)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkePaintUpdatedCallback(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkePaintBitUpdatedCallback(wkeWebView webView, void* param, const void* buffer, const wkeRect* r, int width, int height)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeAlertBoxCallback(wkeWebView webView, void* param, const wkeString msg)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		bool WebView::wkeConfirmBoxCallback(wkeWebView webView, void* param, const wkeString msg)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return false;
			}
			return true;
		}
		bool WebView::wkePromptBoxCallback(wkeWebView webView, void* param, const wkeString msg, const wkeString defaultResult, wkeString result)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return false;
			}
			return true;
		}
		bool WebView::wkeNavigationCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return false;
			}
			return true;
		}
		wkeWebView WebView::wkeCreateViewCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures)
		{
			return wkeWebView();
		}
		void WebView::wkeDocumentReadyCallback(wkeWebView webView, void* param)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeDocumentReady2Callback(wkeWebView webView, void* param, wkeWebFrameHandle frameId)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeLoadingFinishCallback(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		bool WebView::wkeDownloadCallback(wkeWebView webView, void* param, const char* url)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return false;
			}
			return true;
		}
		wkeDownloadOpt WebView::wkeDownload2Callback(wkeWebView webView, void* param, size_t expectedContentLength, const char* url, const char* mime, const char* disposition, wkeNetJob job, wkeNetJobDataBind* dataBind)
		{
			return wkeDownloadOpt();
		}
		void WebView::wkeConsoleCallback(wkeWebView webView, void* param, wkeConsoleLevel level, const wkeString message, const wkeString sourceName, unsigned sourceLine, const wkeString stackTrace)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeOnCallUiThread(wkeWebView webView, void* paramOnInThread)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(paramOnInThread);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeCallUiThread(wkeWebView webView,  void(*func)(wkeWebView, void*), void* param)
		{
		}
		wkeMediaPlayer WebView::wkeMediaPlayerFactory(wkeWebView webView, wkeMediaPlayerClient client, void* npBrowserFuncs, void* npPluginFuncs)
		{
			return wkeMediaPlayer();
		}
		bool WebView::wkeOnIsMediaPlayerSupportsMIMEType(const utf8* mime)
		{
			return false;
		}
		bool WebView::wkeLoadUrlBeginCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job)
		{
			return false;
		}
		void WebView::wkeLoadUrlEndCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job, void* buf, int len)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeLoadUrlFailCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeDidCreateScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int extensionGroup, int worldId)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeWillReleaseScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int worldId)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		bool WebView::wkeNetResponseCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job)
		{
			return false;
		}
		void WebView::wkeOnNetGetFaviconCallback(wkeWebView webView, void* param, const utf8* url, wkeMemBuf* buf)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		bool WebView::wkeWindowClosingCallback(wkeWebView webWindow, void* param)
		{
			return false;
		}
		void WebView::wkeWindowDestroyCallback(wkeWebView webWindow, void* param)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeDraggableRegionsChangedCallback(wkeWebView webView, void* param, const wkeDraggableRegion* rects, int rectCount)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeWillMediaLoadCallback(wkeWebView webView, void* param, const char* url, wkeMediaLoadInfo* info)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeStartDraggingCallback(wkeWebView webView, void* param, wkeWebFrameHandle frame, const wkeWebDragData* data, wkeWebDragOperationsMask mask, const void* image, const wkePoint* dragImageOffset)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeOnPrintCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* printParams)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		void WebView::wkeOnOtherLoadCallback(wkeWebView webView, void* param, wkeOtherLoadType type, wkeTempCallbackInfo* info)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
		bool WebView::wkeOnContextMenuItemClickCallback(wkeWebView webView, void* param, wkeOnContextMenuItemClickType type, wkeOnContextMenuItemClickStep step, wkeWebFrameHandle frameId, void* info)
		{
			return false;
		}
		void WebView::wkeOnPluginFindCallback(wkeWebView webView, void* param, const utf8* mime, void* initializeFunc, void* getEntryPointsFunc, void* shutdownFunc)
		{
			WebView* pWebView = reinterpret_cast<WebView*>(param);
			if (!pWebView)
			{
				return;
			}
		}
}	// namespace GUI
}	// namespace PFD
