#include "Utils.h"
#include "windows.h"
#include <Commom/STLIncluding.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <Commom/stb_image.h>

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

		std::string LoadStringFromFile(const std::string& path)
		{
			std::ifstream	fs(path);
			std::stringstream	ss;
			ss << fs.rdbuf();
			std::string source = ss.str();
			return source;
		}

		osg::ref_ptr<osg::Image> LoadImageFromPath(const std::string& path)
		{
			int width = 0, height = 0, nrChannels = 0;
			unsigned char* imgObj = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
			if (imgObj)
			{
				osg::ref_ptr<osg::Image>	osgIMG = new osg::Image();
				osgIMG->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, imgObj, osg::Image::USE_MALLOC_FREE);
				return osgIMG;
			}
			return nullptr;
		}

		osg::ref_ptr<osg::Image> LoadImageFromMemory(unsigned char* pcBuffer)
		{
			
			if (!pcBuffer)
			{
				return nullptr;
			}
			size_t bufferSize = sizeof(pcBuffer);
			int width = 0, height = 0, nrChannels = 0;
			unsigned char* imgObj = stbi_load_from_memory(pcBuffer, bufferSize, &width, &height, &nrChannels, STBI_rgb_alpha);
			if (imgObj)
			{
				osg::ref_ptr<osg::Image>	osgIMG = new osg::Image();
				osgIMG->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, imgObj, osg::Image::USE_MALLOC_FREE);
				return osgIMG;
			}
			return nullptr;
		}

	}
}