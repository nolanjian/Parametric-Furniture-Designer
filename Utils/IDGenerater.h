#pragma once
#include "UtilsExport.h"
#include <atomic>

class IDGenerater
{
public:
	static UTILS_API IDGenerater* Get();
	IDGenerater();
	~IDGenerater();
	unsigned long long int UTILS_API GetNext();
private:
	std::atomic<unsigned long long int>	m_next = 10000;
};