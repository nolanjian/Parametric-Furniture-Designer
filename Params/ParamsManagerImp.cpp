#include "pch.h"
#include "ParamsManagerImp.h"
#include <cassert>
#include "../Utils/IDGenerater.h"

CParamsManager::CParamsManager()
{
}

std::shared_ptr<CParams> CParamsManager::GetParams(const unsigned long long int id)
{
	assert(id > 0);
	if (m_params.find(id) == m_params.end())
	{
		return nullptr;
	}

	return m_params[id];
}

std::shared_ptr<CParams> CParamsManager::GenParams(unsigned long long int& id)
{
	id = IDGenerater::Get()->GetNext();
	assert(m_params.find(id) == m_params.end() && m_relation.find(id) == m_relation.end() && m_relation.size() == m_params.size());
	m_params[id] = std::make_shared<CParams>(id);
	m_relation[id] = id;
	assert(m_relation.size() == m_params.size());
	return m_params[id];
}

void CParamsManager::Remove(const unsigned long long int id)
{
	assert(id > 0 && m_relation.size() == m_params.size());
	if (m_params.find(id) != m_params.end())
	{
		m_params.erase(id);
	}
	if (m_relation.find(id) != m_relation.end())
	{
		m_relation.erase(id);
	}
	assert(m_relation.size() == m_params.size());
}

unsigned long long int CParamsManager::GetParent(unsigned long long int id)
{
	assert(id > 0 && m_relation.size() == m_params.size());
	if (m_relation.find(id) != m_relation.end())
	{
		return m_relation[id];
	}
	return 0;
}

bool CParamsManager::SetParent(unsigned long long int id, unsigned long long int parent)
{
	assert(id > 0 && parent > 0 && m_relation.size() == m_params.size());
	if (m_relation.find(id) != m_relation.end() && m_relation.find(parent) != m_relation.end())
	{
		m_relation[id] = parent;
		return true;
	}
	return false;
}
