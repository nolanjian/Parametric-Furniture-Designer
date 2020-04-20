#include "pch.h"
#include "ParametricComponent.h"
#include "../Utils/IParamsConverter.h"

bool ParametricComponent::Update()
{
	bool bRet = UpdateFormulas();
	return bRet;
}

bool ParametricComponent::UpdateFormulas()
{
	if (!ReInitParser() || !SetParentFormulars())
	{
		return false;
	}

	if (!UpdateSelfFormulas())
	{
		return false;
	}

	for (auto& child : _children)
	{
		ParametricComponent* pc = dynamic_cast<ParametricComponent*>(child.get());
		if (pc)
		{
			pc->UpdateFormulas();
		}
	}

	return InitFromParams();
}


//bool ParametricComponent::UpdateFormulas()
//{
//	if (!ReInitParser() || !SetParentFormulars())
//	{
//		return false;
//	}
//
//	if (!UpdateSelfFormulas())
//	{
//		return false;
//	}
//
//	InitFromParams();
//
//	bool bUpdateALL = true;
//	for (auto& child : _children)
//	{
//		ParametricComponent* pBaseObject = dynamic_cast<ParametricComponent*>(child.get());
//		if (pBaseObject)
//		{
//			bUpdateALL &= pBaseObject->UpdateFormulas();
//		}
//	}
//	return bUpdateALL;
//}

bool ParametricComponent::UpdateSelfFormulas()
{
	assert(m_parser);
	m_formulasResult.clear();

	std::list<std::string>	lstFormulas;

	for (auto pKV : GetFormulas())
	{
		static std::string wsParent(_T("Parent."));
		static std::string wsrParent(_T("Parent_"));
		std::string val = pKV.second;

		size_t	pos = val.find(wsParent);
		while (pos != std::string::npos)
		{
			val = val.replace(pos, wsParent.length(), wsrParent);
			pos = val.find(wsParent);
		}

		std::string line = pKV.first + _T("=") + val;
		lstFormulas.push_back(line);
	}

	size_t	preSize = lstFormulas.size();
	while (!lstFormulas.empty())
	{
		auto itr = lstFormulas.begin();
		while (itr != lstFormulas.end())
		{
			if (SetOneLine(*itr))
			{
				itr = lstFormulas.erase(itr);
			}
			else
			{
				++itr;
			}
		}
		if (lstFormulas.size() == preSize)
		{
			std::string	outMsg = _T("Parsing Fail for:\n");
			for (auto& s : lstFormulas)
			{
				outMsg += s + _T("\n");
			}
			LOG(ERROR) << outMsg;
			return false;
		}
	}

	//assert(m_parser->GetVar().size() >= GetFormulas().size() + (GetParent() ? GetParent()->FormulasResult().size() : 0));

	for (auto& kv : m_formulas)
	{
		if (m_parser->IsVarDefined(kv.first))
		{
			const mup::var_maptype& m = m_parser->GetVar();
			m_formulasResult[kv.first] = m.at(kv.first);
		}
		else
		{
			m_formulasResult.clear();
			assert(false);
			return false;
		}
	}

	return true;
}

bool ParametricComponent::SetOneLine(const std::string& line)
{
	try
	{
		m_parser->SetExpr(line);
		m_parser->Eval();
		return true;
	}
	catch (const mup::ParserError& e)
	{
		LOG(ERROR) << e.GetMsg();
	}
	catch (const std::exception& e)
	{
		LOG(ERROR) << e.what();
	}
	catch (...)
	{
		LOG(ERROR) << "UNKNOW";
	}
	return false;
}

bool ParametricComponent::ParseParams(const nlohmann::json::value_type& params)
{
	if (!params.is_array())
	{
		LOG(ERROR) << "params is not array";
		// TODO LOG
		return false;
	}
	for (const auto& item : params)
	{
		if (item.is_string())
		{
			std::string strFormula = item.get<std::string>();
			auto pos = strFormula.find("=");
			if (pos != std::string::npos)
			{
				m_formulas[_T("name")] = strFormula.substr(0, pos);
				m_formulas[_T("formula")] = strFormula.substr(pos + 1);
			}
			else
			{
				// LOG todo
			}
		}
	}
	assert(params.size() == m_formulas.size());

	return true;
}

bool ParametricComponent::ImportParams(const fx::gltf::Node& node)
{
	m_formulas.clear();
	if (!node.extensionsAndExtras.contains("params"))
	{
		return true;
	}

	const nlohmann::json::value_type& params = node.extensionsAndExtras["params"];
	if (params.is_string())
	{
		std::string	str = params.get<std::string>();
		std::string decodeStr;
		if (IParamsConverter::Decode(str, decodeStr))
		{
			nlohmann::json	paramsJson;
			std::stringstream	ss;
			ss << decodeStr;
			paramsJson << ss;
			return ParseParams(paramsJson);
		}
		else
		{
			return false;
		}
	}
	return ParseParams(params);
}

const mup::var_maptype& ParametricComponent::FormulasResult()
{
	return m_formulasResult;
}

bool ParametricComponent::ReInitParser()
{
	m_parser = std::make_shared<mup::ParserX>(mup::EPackages::pckALL_NON_COMPLEX);
	return m_parser.get() != nullptr;
}

bool ParametricComponent::SetParentFormulars()
{
	osg::ref_ptr<BaseObject> pParent = GetParent();
	if (!pParent.valid())
	{
		return false;
	}

	ParametricComponent* pc = dynamic_cast<ParametricComponent*>(pParent.get());
	if (!pc)
	{
		return true;
	}

	const mup::var_maptype& parentFormulasResult = pc->FormulasResult();
	if (parentFormulasResult.empty())
	{
		return true;
	}

	try
	{
		m_parser->ClearVar();
		assert(m_parser->GetExprVar().size() == 0);
		for (auto pKV : parentFormulasResult)
		{
			mup::IValue* pValue = pKV.second->AsIValue();
			if (pValue == nullptr)
			{
				continue;
			}
			mup::string_type name = _T("Parent_") + pKV.first;


			mup::ptr_tok_type ptt = pKV.second;

			m_parser->DefineVar(name, mup::Variable(pValue));
		}
		assert(parentFormulasResult.size() == m_parser->GetExprVar().size());

		return true;
	}
	catch (const mup::ParserError& e)
	{
		LOG(ERROR) << e.GetMsg();
	}
	catch (const std::exception& e)
	{
		LOG(ERROR) << e.what();
	}
	catch (...)
	{
		LOG(ERROR) << "UNKNOW";
	}
	return false;
}