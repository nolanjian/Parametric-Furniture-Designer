#include "Utils.h"
#include "windows.h"

namespace PFD
{
	namespace Utils
	{
		static bool bInitLogger = []() {
			return InitLogger();
		}();

		bool InitLogger()
		{
			if (bInitLogger)
				return true;
			auto max_size = 1048576 * 5;
			auto max_files = 3;
			auto logger = spdlog::rotating_logger_mt(PFD_LOGGER, L"logs/program.log", max_size, max_files);
			bInitLogger = true;
			return bInitLogger;
		}

		std::wstring Utf8ToUnicode(const std::string& strUTF8)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
			if (len == 0)
			{
				return L"";
			}

			wchar_t* pRes = new wchar_t[len];
			if (pRes == NULL)
			{
				return L"";
			}

			MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pRes, len);
			pRes[len - 1] = L'\0';
			std::wstring result = pRes;
			delete[] pRes;

			return result;
		}

		std::string UnicodeToUtf8(const std::wstring& strUnicode)
		{
			int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
			if (len == 0)
			{
				return "";
			}

			char* pRes = new char[len];
			if (pRes == NULL)
			{
				return "";
			}

			WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
			pRes[len - 1] = '\0';
			std::string result = pRes;
			delete[] pRes;

			return result;
		}

		std::wstring StringToWString(const std::string& str)
		{
			int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			if (len == 0)
			{
				return L"";
			}

			wchar_t* pRes = new wchar_t[len];
			if (pRes == NULL)
			{
				return L"";
			}

			MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pRes, len);
			pRes[len - 1] = L'\0';
			std::wstring result = pRes;
			delete[] pRes;

			return result;
		}

		std::string WStringToString(const std::wstring& wstr)
		{
			int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
			if (len == 0)
			{
				return "";
			}

			char* pRes = new char[len];
			if (pRes == NULL)
			{
				return "";
			}

			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pRes, len, NULL, NULL);
			pRes[len - 1] = '\0';
			std::string result = pRes;
			delete[] pRes;

			return result;
		}
	}
}