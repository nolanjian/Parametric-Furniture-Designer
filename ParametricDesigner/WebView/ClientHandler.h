#pragma once

#include <include/cef_client.h>
#include <include/cef_drag_handler.h>
#include <include/cef_drag_data.h>
//#include "Widget.h"

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			//class Widget;

			class ClientHandler
#pragma region Derived
				: public CefClient
				, public CefAudioHandler
				, public CefDialogHandler
				, public CefContextMenuHandler
				, public CefDisplayHandler
				, public CefLifeSpanHandler
				, public CefLoadHandler
				, public CefDownloadHandler
				, public CefDragHandler
				, public CefFindHandler
				, public CefFocusHandler
				, public CefJSDialogHandler
				, public CefKeyboardHandler
				, public CefRenderHandler
				, public CefRequestHandler
#pragma endregion Derived
			{
			public:
				ClientHandler() {};
				ClientHandler(void* pWidget);
				virtual ~ClientHandler();

				void SetWidget(void* pWidget);
				CefRefPtr<CefBrowser> GetBrowser();

				typedef cef_drag_operations_mask_t DragOperationsMask;
				typedef cef_window_open_disposition_t WindowOpenDisposition;

#pragma region CefClient
				virtual CefRefPtr<CefAudioHandler> GetAudioHandler() override;
				virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
				virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
				virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;
				virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;
				virtual CefRefPtr<CefDialogHandler> GetDialogHandler() override;
				virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
				virtual CefRefPtr<CefDragHandler> GetDragHandler() override;
				virtual CefRefPtr<CefFindHandler> GetFindHandler() override;
				virtual CefRefPtr<CefFocusHandler> GetFocusHandler() override;
				virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
				virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override;
				virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
				virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override;
				virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefFrame> frame,
					CefProcessId source_process,
					CefRefPtr<CefProcessMessage> message) override;
#pragma endregion CefClient

#pragma region CefRequestHandler
				virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect) override;
				virtual bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, WindowOpenDisposition target_disposition, bool user_gesture) override;
				virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
					CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefFrame> frame,
					CefRefPtr<CefRequest> request,
					bool is_navigation,
					bool is_download,
					const CefString& request_initiator,
					bool& disable_default_handling) override;
				virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
					const CefString& origin_url,
					bool isProxy,
					const CefString& host,
					int port,
					const CefString& realm,
					const CefString& scheme,
					CefRefPtr<CefAuthCallback> callback) override;
				virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
					const CefString& origin_url,
					int64 new_size,
					CefRefPtr<CefRequestCallback> callback) override;
				virtual bool OnCertificateError(CefRefPtr<CefBrowser> browser,
					cef_errorcode_t cert_error,
					const CefString& request_url,
					CefRefPtr<CefSSLInfo> ssl_info,
					CefRefPtr<CefRequestCallback> callback) override;
				virtual bool OnSelectClientCertificate(
					CefRefPtr<CefBrowser> browser,
					bool isProxy,
					const CefString& host,
					int port,
					const X509CertificateList& certificates,
					CefRefPtr<CefSelectClientCertificateCallback> callback) override;
				virtual void OnPluginCrashed(CefRefPtr<CefBrowser> browser,
					const CefString& plugin_path) override;
				virtual void OnRenderViewReady(CefRefPtr<CefBrowser> browser) override;
				virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
					TerminationStatus status) override;
#pragma endregion CefRequestHandler

#pragma region CefRenderHandler
				virtual CefRefPtr<CefAccessibilityHandler> GetAccessibilityHandler() override;
				virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
				virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
				virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
					int viewX,
					int viewY,
					int& screenX,
					int& screenY) override;
				virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
					CefScreenInfo& screen_info) override;
				virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
				virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;
				virtual void OnPaint(CefRefPtr<CefBrowser> browser,
					PaintElementType type,
					const RectList& dirtyRects,
					const void* buffer,
					int width,
					int height) override;
				virtual void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
					PaintElementType type,
					const RectList& dirtyRects,
					void* shared_handle) override;
				virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
					CefCursorHandle cursor,
					CursorType type,
					const CefCursorInfo& custom_cursor_info) override;
				virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefDragData> drag_data,
					DragOperationsMask allowed_ops,
					int x,
					int y) override;
				virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
					DragOperation operation) override;
				virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser,
					double x,
					double y) override;
				virtual void OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser,
					const CefRange& selected_range,
					const RectList& character_bounds) override;
				virtual void OnTextSelectionChanged(CefRefPtr<CefBrowser> browser,
					const CefString& selected_text,
					const CefRange& selected_range) override;
				virtual void OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser,
					TextInputMode input_mode) override;
#pragma endregion CefRenderHandler

#pragma region CefKeyboardHandler
				virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
					const CefKeyEvent& event,
					CefEventHandle os_event,
					bool* is_keyboard_shortcut) override;
				virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
					const CefKeyEvent& event,
					CefEventHandle os_event) override;
#pragma endregion CefKeyboardHandler

#pragma region CefJSDialogHandler
				virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
					const CefString& origin_url,
					JSDialogType dialog_type,
					const CefString& message_text,
					const CefString& default_prompt_text,
					CefRefPtr<CefJSDialogCallback> callback,
					bool& suppress_message) override;
				virtual bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
					const CefString& message_text,
					bool is_reload,
					CefRefPtr<CefJSDialogCallback> callback) override;
				virtual void OnResetDialogState(CefRefPtr<CefBrowser> browser) override;
				virtual void OnDialogClosed(CefRefPtr<CefBrowser> browser) override;
#pragma endregion CefJSDialogHandler

#pragma region CefFocusHandler
				virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;
				virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) override;
				virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) override;
#pragma endregion CefFocusHandler

#pragma region CefFindHandler
				virtual void OnFindResult(CefRefPtr<CefBrowser> browser,
					int identifier,
					int count,
					const CefRect& selectionRect,
					int activeMatchOrdinal,
					bool finalUpdate) override;
#pragma endregion CefFindHandler

#pragma region CefDragHandler
				virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefDragData> dragData,
					DragOperationsMask mask) override;
				virtual void OnDraggableRegionsChanged(
					CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefFrame> frame,
					const std::vector<CefDraggableRegion>& regions) override;
#pragma endregion CefDragHandler

#pragma region CefDownloadHandler
				virtual void OnBeforeDownload(
					CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefDownloadItem> download_item,
					const CefString& suggested_name,
					CefRefPtr<CefBeforeDownloadCallback> callback) override;
				virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefDownloadItem> download_item,
					CefRefPtr<CefDownloadItemCallback> callback) override;
#pragma endregion CefDownloadHandler

#pragma region CefDialogHandler
				virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
					FileDialogMode mode,
					const CefString& title,
					const CefString& default_file_path,
					const std::vector<CefString>& accept_filters,
					int selected_accept_filter,
					CefRefPtr<CefFileDialogCallback> callback) override;
#pragma endregion CefDialogHandler

#pragma region CefAudioHandler
				virtual bool GetAudioParameters(CefRefPtr<CefBrowser> browser
					, CefAudioParameters& params) override;
				virtual void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser
					, const CefAudioParameters& params, int channels) override;
				virtual void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser
					, const float** data, int frames, int64 pts) override;
				virtual void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser) override;
				virtual void OnAudioStreamError(CefRefPtr<CefBrowser> browser
					, const CefString& message) override;
#pragma endregion CefAudioHandler

#pragma region CefContextMenuHandler
				virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, CefRefPtr<CefContextMenuParams> params
					, CefRefPtr<CefMenuModel> model) override;
				virtual bool RunContextMenu(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, CefRefPtr<CefContextMenuParams> params
					, CefRefPtr<CefMenuModel> model
					, CefRefPtr<CefRunContextMenuCallback> callback) override;
				virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, CefRefPtr<CefContextMenuParams> params
					, int command_id
					, EventFlags event_flags) override;
				virtual void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame) override;
#pragma endregion CefContextMenuHandler

#pragma region CefDisplayHandler
				virtual void OnAddressChange(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, const CefString& url) override;
				virtual void OnTitleChange(CefRefPtr<CefBrowser> browser
					, const CefString& title) override;
				virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser
					, const std::vector<CefString>& icon_urls) override;
				virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser
					, bool fullscreen) override;
				virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser
					, bool isLoading
					, bool canGoBack
					, bool canGoForward) override;
				virtual bool OnTooltip(CefRefPtr<CefBrowser> browser
					, CefString& text) override;
				virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser
					, const CefString& value) override;
				virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser
					, cef_log_severity_t level
					, const CefString& message
					, const CefString& source
					, int line) override;
				virtual bool OnAutoResize(CefRefPtr<CefBrowser> browser
					, const CefSize& new_size) override;
				virtual void OnLoadingProgressChange(CefRefPtr<CefBrowser> browser
					, double progress) override;
#pragma endregion CefDisplayHandler

#pragma region CefLifeSpanHandler
				virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
					CefRefPtr<CefFrame> frame,
					const CefString& target_url,
					const CefString& target_frame_name,
					WindowOpenDisposition target_disposition,
					bool user_gesture,
					const CefPopupFeatures& popupFeatures,
					CefWindowInfo& windowInfo,
					CefRefPtr<CefClient>& client,
					CefBrowserSettings& settings,
					CefRefPtr<CefDictionaryValue>& extra_info,
					bool* no_javascript_access) override;
				virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
				virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
				virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
#pragma endregion CefLifeSpanHandler

#pragma region CefLoadHandler
				virtual void OnLoadStart(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, TransitionType transition_type) override;
				virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, int httpStatusCode) override;
				virtual void OnLoadError(CefRefPtr<CefBrowser> browser
					, CefRefPtr<CefFrame> frame
					, ErrorCode errorCode
					, const CefString& errorText
					, const CefString& failedUrl) override;
#pragma endregion CefLoadHandler

			private:
				CefRefPtr<CefBrowser> m_browser;
				int m_browserId = 0;
				void* m_pWidget = nullptr;
				IMPLEMENT_REFCOUNTING(ClientHandler);
			};
		}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD