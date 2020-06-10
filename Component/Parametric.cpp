#include "Parametric.h"
#include "../Utils/IParamsConverter.h"

namespace PFD
{
	namespace Component
	{
		std::shared_ptr<spdlog::logger> Parametric::logger = spdlog::get(PFD_LOGGER);

		bool Parametric::Update()
		{
			bool bRet = UpdateFormulas();
			return bRet;
		}

		bool Parametric::UpdateFormulas()
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
				Parametric* pc = dynamic_cast<Parametric*>(child.get());
				if (pc)
				{
					pc->UpdateFormulas();
				}
			}

			return InitFromParams();
		}

		bool Parametric::UpdateSelfFormulas()
		{
			assert(m_parser);
			m_parser->EnableAutoCreateVar(true);
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

		bool Parametric::SetOneLine(const std::string& line)
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
				logger->error(e.what());
			}
			catch (...)
			{
				logger->error(_T("UNKNOW"));
			}
			return false;
		}

		bool Parametric::ParseParams(const nlohmann::json::value_type& params)
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
					std::string unicodeFormula = utf8Formula;

					std::pair<std::string, std::string>	pair;
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

		bool Parametric::ImportParams(const fx::gltf::Node& node)
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
				if (PFD::Utils::IParamsConverter::Decode(str, decodeStr))
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

		bool Parametric::regexParseFormular(const std::string& strFormular, std::string& strKey, std::string& strVal)
		{
			try
			{
				std::regex	patternReplace(_T("\\s+"));
				std::string strF = std::regex_replace(strFormular, patternReplace, _T(""));

				std::regex	pattern(_T("(.+)=(.+)"));
				std::smatch	result;

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
				logger->error(err.what());
			}
			catch (const std::exception& err)
			{
				logger->error(err.what());
			}
			return false;
		}

		bool Parametric::regexParseKV(std::string& strKV)
		{
			try
			{
				std::regex	pattern(_T("\\W*(\\w+)\\W*"));

				std::smatch	result;

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
				logger->error(err.what());
			}
			catch (const std::exception& err)
			{
				logger->error(err.what());
			}
			return false;
		}

		bool Parametric::SetParam(const std::string& strFormular)
		{
			std::pair<std::string, std::string>	pair;
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

		bool Parametric::SetParam(std::string& strKey, std::string& strValue)
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

		std::string Parametric::GetParam(const std::string& strName)
		{
			if (m_formulas.find(strName) != m_formulas.end())
			{
				return m_formulas[strName];
			}
			return _T("");
		}

		std::string Parametric::GetParamResult(const std::string& strName)
		{
			if (m_formulasResult.find(strName) == m_formulasResult.end())
			{
				return _T("");
			}
			auto val = m_formulasResult[strName];

			mup::IValue* pValue = val->AsIValue();

			std::string str;
			if (pValue->IsString())
			{
				str = pValue->GetString();
				return str;
			}
			else if (pValue->IsVariable())
			{
				double fVal = pValue->GetFloat();
				return std::to_string(fVal);
			}
			else if (pValue->IsInteger())
			{
				int nVal = pValue->GetInteger();
				return std::to_string(nVal);
			}

			throw std::exception("unhandled data type");
		}

		bool Parametric::GetFormulaPairFromString(const std::string& str, std::pair<std::string, std::string>& pair)
		{
			std::string strKey, strVal;
			if (regexParseFormular(str, strKey, strVal))
			{
				pair.first = strKey;
				pair.second = strVal;
				return true;
			}

			return false;
		}

		const mup::var_maptype& Parametric::FormulasResult()
		{
			return m_formulasResult;
		}

		bool Parametric::ReInitParser()
		{
			m_parser = std::make_shared<mup::ParserX>(mup::EPackages::pckALL_NON_COMPLEX);
			m_parser->EnableAutoCreateVar(true);
			return m_parser.get() != nullptr;
		}

		bool Parametric::SetParentFormulars()
		{
			if (getNumParents() < 1)
			{
				return true;
			}

			Parametric* pParent = dynamic_cast<Parametric*>(getParent(0));
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
					std::string name = _T("Parent_") + pKV.first;


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
				logger->error(e.what());
			}
			catch (...)
			{
				logger->error(_T("UNKNOW"));
			}
			return false;
		}
	}
}