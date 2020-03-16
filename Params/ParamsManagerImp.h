#pragma once
#include "IParamsManager.h"
#include <map>

class CParamsManager : public IParamsManager
{
public:
	CParamsManager();

	virtual std::shared_ptr<CParams> GetParams(const unsigned long long int id) override;
	virtual std::shared_ptr<CParams> GenParams(unsigned long long int& id) override;
	virtual void Remove(const unsigned long long int id) override;
	virtual unsigned long long int GetParent(unsigned long long int id) override;
	virtual bool SetParent(unsigned long long int id, unsigned long long int parent) override;

protected:
private:
	std::map<unsigned long long int, std::shared_ptr<CParams>>	m_params;
	std::map<unsigned long long int, unsigned long long int>	m_relation;
};