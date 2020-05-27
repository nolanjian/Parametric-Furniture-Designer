﻿
#include "IDGenerater.h"

namespace PFD
{

	namespace Utils
	{

		IDGenerater* IDGenerater::Get()
		{
			static IDGenerater s_obj;
			return &s_obj;
		}

		IDGenerater::IDGenerater()
		{
		}

		IDGenerater::~IDGenerater()
		{
		}

		unsigned long long int IDGenerater::GetNext()
		{
			unsigned long long int ret = m_next;
			++m_next;
			return ret;
		}

	}
}