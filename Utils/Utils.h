// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the UTILS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// PFD_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include "PFDUtilsExport.h"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define PFD_LOGGER "pfd_async_file_logger"

namespace PFD
{
	namespace Utils
	{
		PFD_UTILS_API bool InitLogger();

		PFD_UTILS_API std::wstring Utf8ToUnicode(const std::string& strUTF8);

		PFD_UTILS_API std::string UnicodeToUtf8(const std::wstring& strUnicode);

		PFD_UTILS_API std::wstring StringToWString(const std::string& str);

		PFD_UTILS_API std::string WStringToString(const std::wstring& wstr);
	}	// namespace Utils
}	// namespace PFD


