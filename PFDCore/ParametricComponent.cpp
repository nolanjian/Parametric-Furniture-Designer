#include "pch.h"
#include "ParametricComponent.h"
#include "../Utils/IParamsConverter.h"

namespace PFDCore
{
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

	bool ParametricComponent::UpdateSelfFormulas()
	{
		assert(m_parser);
		m_parser->EnableAutoCreateVar(true);
		m_formulasResult.clear();

		std::list<mup::string_type>	lstFormulas;

		for (auto pKV : GetFormulas())
		{
			static mup::string_type wsParent(_T("Parent."));
			static mup::string_type wsrParent(_T("Parent_"));
			mup::string_type val = pKV.second;

			size_t	pos = val.find(wsParent);
			while (pos != mup::string_type::npos)
			{
				val = val.replace(pos, wsParent.length(), wsrParent);
				pos = val.find(wsParent);
			}

			mup::string_type line = pKV.first + _T("=") + val;
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
				mup::string_type	outMsg = _T("Parsing Fail for:\n");
				for (auto& s : lstFormulas)
				{
					outMsg += s + _T("\n");
				}
				
				logger->error(outMsg);
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

	bool ParametricComponent::SetOneLine(const mup::string_type& line)
	{
		try
		{
			m_parser->SetExpr(line);
			m_parser->Eval();
			return true;
		}
		catch (const mup::ParserError& e)
		{
			logger->error(e.GetMsg());
		}
		catch (const std::exception& e)
		{
			logger->error(PFDUtils::StringToWString(e.what()));
		}
		catch (...)
		{
			logger->error(_T("UNKNOW"));
		}
		return false;
	}

	bool ParametricComponent::ParseParams(const nlohmann::json::value_type& params)
	{
		if (!params.is_array())
		{
			logger->error(_T("params is not array"));
			return false;
		}
		for (const auto& item : params)
		{
			if (item.is_string())
			{
				// eg: W = a + b
				std::string utf8Formula = item.get<std::string>();
				mup::string_type unicodeFormula = PFDUtils::Utf8ToUnicode(utf8Formula);

				std::pair<mup::string_type, mup::string_type>	pair;
				if (!GetFormulaPairFromString(unicodeFormula, pair))
				{
					
					logger->error(_T("Parse Fail:{}"), unicodeFormula);
					continue;
				}

				if (m_formulas.find(pair.first) != m_formulas.end())
				{
					logger->error(_T("Dup Check:{}"), pair.first);
					continue;
				}

				m_formulas.insert(pair);
			}
		}

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
			if (PFDUtils::IParamsConverter::Decode(str, decodeStr))
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

	bool ParametricComponent::regexParseFormular(const mup::string_type& strFormular, mup::string_type& strKey, mup::string_type& strVal)
	{
		try
		{
			std::wregex	patternReplace(_T("\\s+"));
			mup::string_type strF = std::regex_replace(strFormular, patternReplace, _T(""));

			std::wregex	pattern(_T("(.+)=(.+)"));
			std::wsmatch	result;

			bool bMatch = std::regex_match(strF, result, pattern);
			if (!bMatch)
				return false;
			else if (result.size() < 3)
				return false;

			strKey = result[1];
			strVal = result[2];
			return true;
		}
		catch (const std::regex_error& err)
		{
			logger->error(PFDUtils::StringToWString(err.what()));
		}
		catch (const std::exception& err)
		{
			logger->error(PFDUtils::StringToWString(err.what()));
		}
		return false;
	}

	bool ParametricComponent::regexParseKV(mup::string_type& strKV)
	{
		try
		{
			std::wregex	pattern(_T("\\W*(\\w+)\\W*"));

			std::wsmatch	result;

			bool bMatch = std::regex_match(strKV, result, pattern);
			if (!bMatch)
				return false;
			else if (result.size() < 2)
				return false;

			strKV = result[1];
			return true;
		}
		catch (const std::regex_error& err)
		{
			logger->error(PFDUtils::StringToWString(err.what()));
		}
		catch (const std::exception& err)
		{
			logger->error(PFDUtils::StringToWString(err.what()));
		}
		return false;
	}

	bool ParametricComponent::SetParam(const mup::string_type& strFormular)
	{
		std::pair<mup::string_type, mup::string_type>	pair;
		if (!GetFormulaPairFromString(strFormular, pair))
		{
			logger->error(_T("Parse Fail:{}"), strFormular);
			return false;
		}

		if (m_formulas.find(pair.first) != m_formulas.end())
		{
			logger->warn(_T("Dup Check:{}"), strFormular);
		}

		m_formulas.insert(pair);

		return true;
	}

	bool ParametricComponent::SetParam(mup::string_type& strKey, mup::string_type& strValue)
	{
		if (!regexParseKV(strKey))
		{
			logger->error(_T("Parse Key /{}/ fail"), strKey);
			return false;
		}
		if (!regexParseKV(strValue))
		{
			logger->error(_T("Parse Value /{}/ fail"), strValue);
			return false;
		}
		m_formulas[strKey] = strValue;
		return true;
	}

	mup::string_type ParametricComponent::GetParam(const mup::string_type& strName)
	{
		if (m_formulas.find(strName) != m_formulas.end())
		{
			return m_formulas[strName];
		}
		return _T("");
	}

	mup::string_type ParametricComponent::GetParamResult(const mup::string_type& strName)
	{
		if (m_formulasResult.find(strName) == m_formulasResult.end())
		{
			return _T("");
		}
		auto val = m_formulasResult[strName];

		mup::IValue* pValue = val->AsIValue();

		mup::string_type str;
		if (pValue->IsString())
		{
			str = pValue->GetString();
			return str;
		}
		else if (pValue->IsVariable())
		{
			double fVal = pValue->GetFloat();
			return std::to_wstring(fVal);
		}
		else if (pValue->IsInteger())
		{
			int nVal = pValue->GetInteger();
			return std::to_wstring(nVal);
		}

		throw new std::exception("unhandled data type");
	}

	bool ParametricComponent::GetFormulaPairFromString(const mup::string_type& str, std::pair<mup::string_type, mup::string_type>& pair)
	{
		mup::string_type strKey, strVal;
		if (regexParseFormular(str, strKey, strVal))
		{
			pair.first = strKey;
			pair.second = strVal;
			return true;
		}

		return false;
	}

	const mup::var_maptype& ParametricComponent::FormulasResult()
	{
		return m_formulasResult;
	}

	bool ParametricComponent::ReInitParser()
	{
		m_parser = std::make_shared<mup::ParserX>(mup::EPackages::pckALL_NON_COMPLEX);
		m_parser->EnableAutoCreateVar(true);
		return m_parser.get() != nullptr;
	}

	bool ParametricComponent::SetParentFormulars()
	{
		if (getNumParents() < 1)
		{
			return true;
		}

		ParametricComponent* pParent = dynamic_cast<ParametricComponent*>(getParent(0));
		if (!pParent)
		{
			return true;
		}

		const mup::var_maptype& parentFormulasResult = pParent->FormulasResult();
		if (parentFormulasResult.empty())
		{
			return true;
		}

		try
		{
			m_parser->ClearVar();

			const mup::var_maptype& exprVar = m_parser->GetVar();

			assert(exprVar.size() == 0);
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
			assert(parentFormulasResult.size() == exprVar.size());

			return true;
		}
		catch (const mup::ParserError& e)
		{
			logger->error(e.GetMsg());
		}
		catch (const std::exception& e)
		{
			logger->error(PFDUtils::StringToWString(e.what()));
		}
		catch (...)
		{
			logger->error(_T("UNKNOW"));
		}
		return false;
	}
}