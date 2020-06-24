#pragma once
#include <Utils/Export.h>
#include <string>

namespace PFD
{

	namespace Utils
	{
		class PFD_UTILS_API IParamsConverter
		{
		public:
			static bool Decode(const std::string& src, std::string& target);
			static bool Encode(const std::string& src, std::string& target);
		protected:
		private:
		};
	}
}