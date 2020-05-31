#include "ClientHandler.h"

namespace PFD
{
	namespace GUI
	{
		namespace WebView
		{
			ClientHandler::~ClientHandler()
			{
			}

			CefRefPtr<CefAudioHandler> ClientHandler::GetAudioHandler()
			{
				return this;
			}

			CefRefPtr<CefContextMenuHandler> ClientHandler::GetContextMenuHandler()
			{
				return this;
			}

			CefRefPtr<CefLifeSpanHandler> ClientHandler::GetLifeSpanHandler()
			{
				return this;
			}

			CefRefPtr<CefLoadHandler> ClientHandler::GetLoadHandler()
			{
				return this;
			}

			CefRefPtr<CefDisplayHandler> ClientHandler::GetDisplayHandler()
			{
				return this;
			}

			CefRefPtr<CefDialogHandler> ClientHandler::GetDialogHandler()
			{
				return this;
			}

			CefRefPtr<CefDownloadHandler> ClientHandler::GetDownloadHandler()
			{
				return this;
			}

			CefRefPtr<CefDragHandler> ClientHandler::GetDragHandler()
			{
				return this;
			}

			CefRefPtr<CefFindHandler> ClientHandler::GetFindHandler()
			{
				return this;
			}

			CefRefPtr<CefFocusHandler> ClientHandler::GetFocusHandler()
			{
				return this;
			}

			CefRefPtr<CefJSDialogHandler> ClientHandler::GetJSDialogHandler()
			{
				return this;
			}

			CefRefPtr<CefKeyboardHandler> ClientHandler::GetKeyboardHandler()
			{
				return this;
			}

			CefRefPtr<CefRenderHandler> ClientHandler::GetRenderHandler()
			{
				return this;
			}

			CefRefPtr<CefRequestHandler> ClientHandler::GetRequestHandler()
			{
				return this;
			}

			bool ClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
			{
				return false;
			}

			bool ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect)
			{
				return false;
			}

			bool ClientHandler::OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, WindowOpenDisposition target_disposition, bool user_gesture)
			{
				return false;
			}

			CefRefPtr<CefResourceRequestHandler> ClientHandler::GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_navigation, bool is_download, const CefString& request_initiator, bool& disable_default_handling)
			{
				return CefRefPtr<CefResourceRequestHandler>();
			}

			bool ClientHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser, const CefString& origin_url, bool isProxy, const CefString& host, int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback)
			{
				return false;
			}

			bool ClientHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser, const CefString& origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback)
			{
				return false;
			}

			bool ClientHandler::OnCertificateError(CefRefPtr<CefBrowser> browser, cef_errorcode_t cert_error, const CefString& request_url, CefRefPtr<CefSSLInfo> ssl_info, CefRefPtr<CefRequestCallback> callback)
			{
				return false;
			}

			bool ClientHandler::OnSelectClientCertificate(CefRefPtr<CefBrowser> browser, bool isProxy, const CefString& host, int port, const X509CertificateList& certificates, CefRefPtr<CefSelectClientCertificateCallback> callback)
			{
				return false;
			}

			void ClientHandler::OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path)
			{
			}

			void ClientHandler::OnRenderViewReady(CefRefPtr<CefBrowser> browser)
			{
			}

			void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status)
			{
			}

			CefRefPtr<CefAccessibilityHandler> ClientHandler::GetAccessibilityHandler()
			{
				return CefRefPtr<CefAccessibilityHandler>();
			}

			bool ClientHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
			{
				return false;
			}

			void ClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
			{
			}

			bool ClientHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
			{
				return false;
			}

			bool ClientHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
			{
				return false;
			}

			void ClientHandler::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
			{
			}

			void ClientHandler::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
			{
			}

			void ClientHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
			{
			}

			void ClientHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation)
			{
			}

			void ClientHandler::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y)
			{
			}

			void ClientHandler::OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser, const CefRange& selected_range, const RectList& character_bounds)
			{
			}

			void ClientHandler::OnTextSelectionChanged(CefRefPtr<CefBrowser> browser, const CefString& selected_text, const CefRange& selected_range)
			{
			}

			void ClientHandler::OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser, TextInputMode input_mode)
			{
			}

			bool ClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut)
			{
				return false;
			}

			bool ClientHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
			{
				return false;
			}

			bool ClientHandler::OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url, JSDialogType dialog_type, const CefString& message_text, const CefString& default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
			{
				return false;
			}

			bool ClientHandler::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser, const CefString& message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback)
			{
				return false;
			}

			void ClientHandler::OnResetDialogState(CefRefPtr<CefBrowser> browser)
			{
			}

			void ClientHandler::OnDialogClosed(CefRefPtr<CefBrowser> browser)
			{
			}

			void ClientHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next)
			{
			}

			bool ClientHandler::OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source)
			{
				return false;
			}

			void ClientHandler::OnGotFocus(CefRefPtr<CefBrowser> browser)
			{
			}

			void ClientHandler::OnFindResult(CefRefPtr<CefBrowser> browser, int identifier, int count, const CefRect& selectionRect, int activeMatchOrdinal, bool finalUpdate)
			{
			}

			bool ClientHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask)
			{
				return false;
			}

			void ClientHandler::OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const std::vector<CefDraggableRegion>& regions)
			{
			}

			void ClientHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
			{
			}

			void ClientHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback)
			{
			}

			bool ClientHandler::OnFileDialog(CefRefPtr<CefBrowser> browser, FileDialogMode mode, const CefString& title, const CefString& default_file_path, const std::vector<CefString>& accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback)
			{
				return false;
			}

			bool ClientHandler::GetAudioParameters(CefRefPtr<CefBrowser> browser, CefAudioParameters& params)
			{
				return true;
			}

			void ClientHandler::OnAudioStreamStarted(CefRefPtr<CefBrowser> browser, const CefAudioParameters& params, int channels)
			{
			}

			void ClientHandler::OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, const float** data, int frames, int64 pts)
			{
			}

			void ClientHandler::OnAudioStreamStopped(CefRefPtr<CefBrowser> browser)
			{
			}

			void ClientHandler::OnAudioStreamError(CefRefPtr<CefBrowser> browser, const CefString& message)
			{
			}

			void ClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
			{
			}

			bool ClientHandler::RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback)
			{
				return false;
			}

			bool ClientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
			{
				return false;
			}

			void ClientHandler::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
			{
			}

			void ClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
			{
			}

			void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
			{
			}

			void ClientHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls)
			{
			}

			void ClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
			{
			}

			void ClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
			{
			}

			bool ClientHandler::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text)
			{
				return false;
			}

			void ClientHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value)
			{
			}

			bool ClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line)
			{
				return false;
			}

			bool ClientHandler::OnAutoResize(CefRefPtr<CefBrowser> browser, const CefSize& new_size)
			{
				return false;
			}

			void ClientHandler::OnLoadingProgressChange(CefRefPtr<CefBrowser> browser, double progress)
			{
			}

			bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
			{
				return false;
			}

			void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
			{
			}

			bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
			{
				return false;
			}

			void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
			{
			}

			void ClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
			{
			}

			void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
			{
			}

			void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
			{
			}


			void ClientHandler::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, void* shared_handle)
			{
			}

			void ClientHandler::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info)
			{
			}

			bool ClientHandler::StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y)
			{
				return false;
			}

}	// namespace WebView
	}	// namespace GUI
}	// namespace PFD