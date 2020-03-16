#include "pch.h"
#include "ParamsManagerImp.h"

IParamsManager* IParamsManager::Get()
{
	static CParamsManager mgr;
	return &mgr;
}