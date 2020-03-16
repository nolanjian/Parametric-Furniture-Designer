#include "pch.h"
#include "BaseObject.h"
#include "../Utils/IDGenerater.h"
#include "../easyloggingpp/easylogging++.h"

#include <filesystem>

BaseObject::BaseObject()
	:osg::Group()
{
	m_ID = IDGenerater::Get()->GetNext();
}

BaseObject::~BaseObject()
{
}

bool BaseObject::ReInitParser()
{
	m_parser = std::make_shared<mup::ParserX>(mup::EPackages::pckALL_NON_COMPLEX);
	return m_parser.get() != nullptr;
}

bool BaseObject::SetParentFormulars()
{
	osg::ref_ptr<BaseObject> pParent = GetParent();
	if (!pParent.valid())
	{
		return false;
	}

	const mup::var_maptype& parentFormulasResult = pParent->FormulasResult();
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
			if (pValue ==  nullptr)
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
	catch (const mup::ParserError & e)
	{
		LOG(ERROR) << e.GetMsg();
	}
	catch (const std::exception & e)
	{
		LOG(ERROR) << e.what();
	}
	catch (...)
	{
		LOG(ERROR) << "UNKNOW";
	}
	return false;
}

bool BaseObject::UpdateFormulas()
{
	if (!ReInitParser() || !SetParentFormulars())
	{
		return false;
	}

	if (!UpdateSelfFormulas())
	{
		return false;
	}
	
	bool bUpdateALL = true;
	for (auto& child : _children)
	{
		BaseObject* pBaseObject = dynamic_cast<BaseObject*>(child.get());
		if (pBaseObject)
		{
			bUpdateALL &= pBaseObject->UpdateFormulas();
		}
	}
	return bUpdateALL;
}

bool BaseObject::UpdateSelfFormulas()
{	
	assert(m_parser);
	m_formulasResult.clear();

	std::list<std::wstring>	lstFormulas;

	for (auto pKV : GetFormulas())
	{
		static std::wstring wsParent(L"Parent.");
		static std::wstring wsrParent(L"Parent_");
		std::wstring val = pKV.second;

		size_t	pos = val.find(wsParent);
		while (pos != std::wstring::npos)
		{
			val = val.replace(pos, wsParent.length(), wsrParent);
			pos = val.find(wsParent);
		}

		std::wstring line = pKV.first + _T("=") + val;
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
			std::wstring	outMsg = _T("Parsing Fail for:\n");
			for (auto& s : lstFormulas)
			{
				outMsg += s + _T("\n");
			}
			LOG(ERROR) << outMsg;
			return false;
		}
	}

	assert(m_parser->GetVar().size() >= GetFormulas().size() + (GetParent() ? GetParent()->FormulasResult().size() : 0));

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

bool BaseObject::SetOneLine(const std::wstring& line)
{
	try
	{
		m_parser->SetExpr(line);
		m_parser->Eval();
		return true;
	}
	catch (const mup::ParserError & e)
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

const mup::var_maptype& BaseObject::FormulasResult()
{
	return m_formulasResult;
}

osg::ref_ptr<BaseObject> BaseObject::JSON2OSG(const std::string& str)
{
	std::stringstream	ss;
	ss << str;
	std::shared_ptr<fx::gltf::Document>	ptrObj = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(ss, std::filesystem::current_path().u8string()));
	return GLTF2OSG(ptrObj);
}

std::string BaseObject::OSG2JSON(osg::ref_ptr<BaseObject> pObj)
{
	std::shared_ptr<fx::gltf::Document>	ptrObj = OSG2GLTF(pObj);
	std::stringstream	ss;
	fx::gltf::Save(*ptrObj, ss, "", false);
	return ss.str();
}

osg::ref_ptr<osg::Group> BaseObject::LoadSceneFromJsonFile(const std::string& strPath)
{
	std::shared_ptr<fx::gltf::Document>	ptrDOC = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(strPath));
	return GLTF2OSG(ptrDOC);
}

osg::ref_ptr<BaseObject> BaseObject::GLTF2OSG(std::shared_ptr<fx::gltf::Document> gltfObject)
{
	if (!gltfObject.get() || gltfObject->scenes.size() < 1 || (gltfObject->scenes[0]).nodes.size() < 1)
	{
		return nullptr;
	}

	osg::ref_ptr<BaseObject> ptrScene = GLTFNode2BaseObject(gltfObject, gltfObject->scenes[0].nodes[0]);
	return ptrScene;
}

std::shared_ptr<fx::gltf::Document> BaseObject::OSG2GLTF(osg::ref_ptr<BaseObject> pObj)
{
	// TODO
	return std::shared_ptr<fx::gltf::Document>();
}

osg::ref_ptr<BaseObject> BaseObject::GLTFNode2BaseObject(std::shared_ptr<fx::gltf::Document> gltfObject, int32_t nodeID)
{
	const fx::gltf::Node& curNode = gltfObject->nodes[nodeID];

	osg::ref_ptr<BaseObject> curRoot = new BaseObject();
	
	curRoot->LoadRotation(curNode);
	curRoot->LoadScale(curNode);
	curRoot->LoadTranslation(curNode);
	curRoot->LoadMatrix(curNode);
	curRoot->LoadParams(curNode.extensionsAndExtras);

	for (auto childID : curNode.children)
	{
		osg::ref_ptr<BaseObject> child = GLTFNode2BaseObject(gltfObject, childID);
		curRoot->addChild(child);
	}
	return curRoot;
}

bool BaseObject::LoadRotation(const fx::gltf::Node& node)
{
	vRotation.x() = node.rotation.at(0);
	vRotation.y() = node.rotation.at(1);
	vRotation.z() = node.rotation.at(2);
	vRotation.w() = node.rotation.at(3);
	return true;
}

bool BaseObject::LoadScale(const fx::gltf::Node& node)
{
	vScale.x() = node.scale.at(0);
	vScale.y() = node.scale.at(1);
	vScale.z() = node.scale.at(2);
	return true;
}

bool BaseObject::LoadTranslation(const fx::gltf::Node& node)
{
	vTranslation.x() = node.translation.at(0);
	vTranslation.y() = node.translation.at(1);
	vTranslation.z() = node.translation.at(2);
	return false;
}

bool BaseObject::LoadMatrix(const fx::gltf::Node& node)
{
	mat.set(node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3]
		, node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7]
		, node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11]
		, node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]);

	return true;
}

bool BaseObject::LoadParams(const fx::gltf::Node& node)
{
	m_formulas.clear();
	if (!node.extensionsAndExtras.contains("params"))
	{
		return true;
	}

	const nlohmann::json::value_type& params = node.extensionsAndExtras["params"];
	if (!params.is_array())
	{
		// TODO LOG
		return false;
	}
	for (const auto& item : params)
	{
		if (item.is_object() && item.contains("name") && item.contains("formula"))
		{
			m_formulas[L"name"] = item["name"].get<std::wstring>();
			m_formulas[L"formula"] = item["formula"].get<std::wstring>();
		}
	}
	assert(params.size() == m_formulas.size());

	return true;
}

bool BaseObject::LoadMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node)
{
	if (node.mesh == -1)
	{
		return true;
	}

	const fx::gltf::Mesh& mesh = gltfObject->meshes[node.mesh];
	for (const auto& primitive : mesh.primitives)
	{
		switch (primitive.mode)
		{
		case fx::gltf::Primitive::Mode::Points:
		case fx::gltf::Primitive::Mode::Lines:
		case fx::gltf::Primitive::Mode::LineLoop:
		case fx::gltf::Primitive::Mode::LineStrip:
		case fx::gltf::Primitive::Mode::Triangles:
		case fx::gltf::Primitive::Mode::TriangleStrip:
		case fx::gltf::Primitive::Mode::TriangleFan:
		default:
			break;
		}
	}

	return false;
}

osg::ref_ptr<osg::Drawable> BaseObject::LoadPrimitive(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive)
{
	if (primitive.indices == -1)
	{
		switch (primitive.mode)
		{
		case fx::gltf::Primitive::Mode::Points:
			break;
		case fx::gltf::Primitive::Mode::Lines:
			break;
		case fx::gltf::Primitive::Mode::LineLoop:
			break;
		case fx::gltf::Primitive::Mode::LineStrip:
			break;
		case fx::gltf::Primitive::Mode::Triangles:
			break;
		case fx::gltf::Primitive::Mode::TriangleStrip:
			break;
		case fx::gltf::Primitive::Mode::TriangleFan:
			break;
		default:
			assert(false);
			return nullptr;
		}
	}
	else
	{

	}
	primitive;
}
