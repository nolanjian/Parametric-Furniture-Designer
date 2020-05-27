#pragma once
#include "PFDUtilsExport.h"
#include <atomic>

namespace PFD
{

	namespace Utils
	{
		class PFD_UTILS_API IDGenerater
		{
		public:
			static IDGenerater* Get();
			IDGenerater();
			~IDGenerater();
			unsigned long long int GetNext();
		private:
			std::atomic<unsigned long long int>	m_next = 10000;
		};
	}
}