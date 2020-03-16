// Params.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Params.h"

#include <cassert>

CParams::CParams(unsigned long long int id)
    :m_id(id)
{
    assert(m_id > 0);
}

CParams::CParams(unsigned long long int id, const std::map<std::string, std::string>& params)
    : m_id(id), m_params(params)
{
}

bool CParams::Valid()
{
    return m_id != 0;
}

void CParams::Remove(const std::string& key)
{
    if (m_params.find(key) != m_params.end())
    {
        m_params.erase(key);
    }
}

void CParams::SetKV(const std::string& key, const std::string& value)
{
    m_params[key] = value;
}
