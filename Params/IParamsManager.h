#pragma once
#include "ParamsExport.h"

#include <memory>
#include <string>

#include "Params.h"

class PARAMS_API IParamsManager
{
public:
	IParamsManager* Get();
	virtual std::shared_ptr<CParams> GetParams(const unsigned long long int id) = 0;
	virtual std::shared_ptr<CParams> GenParams(unsigned long long int& id) = 0;
	virtual void Remove(const unsigned long long int id) = 0;
	virtual unsigned long long int GetParent(unsigned long long int id) = 0;
	virtual bool SetParent(unsigned long long int id, unsigned long long int parent) = 0;
};