#include "pch.h"
#include "BaseObject.h"
#include "../Utils/IDGenerater.h"
#include "../Utils/IParamsConverter.h"
#include "../easyloggingpp/easylogging++.h"
#include "ObjectFactory.h"
#include "SceneMgr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

BaseObject::BaseObject()
	:osg::MatrixTransform()
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

	InitFromParams();

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

const mup::var_maptype& BaseObject::FormulasResult()
{
	return m_formulasResult;
}

osg::ref_ptr<BaseObject> BaseObject::JSON2OSG(const std::string& str)
{
	try
	{
		std::stringstream	ss;
		ss << str;
		std::shared_ptr<fx::gltf::Document>	ptrObj = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(ss, fx::gltf::detail::GetDocumentRootPath(str)));
		return GLTF2OSG(ptrObj, std::filesystem::current_path().string());
	}
	catch (const std::exception & e)
	{
		LOG(ERROR) << e.what();
	}
	catch (...)
	{
		LOG(ERROR) << "JSON2OSG unknown exception";
	}
	return nullptr;
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
	try
	{
		std::shared_ptr<fx::gltf::Document>	ptrDOC;
		if (strPath.find(".glb") == std::string::npos)
		{
			ptrDOC = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(strPath));
		}
		else
		{
			ptrDOC = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromBinary(strPath));
		}
		return GLTF2OSG(ptrDOC, strPath);
	}
	catch (const std::exception & e)
	{
		LOG(ERROR) << e.what();
	}
	catch (...)
	{
		LOG(ERROR) << "JSON2OSG unknown exception";
	}
	return nullptr;
}

osg::ref_ptr<BaseObject> BaseObject::GLTF2OSG(std::shared_ptr<fx::gltf::Document> gltfObject, const std::string& strPath)
{
	osg::ref_ptr<BaseObject> ptrScene = ObjectFactory::CreateObject(ObjectFactory::SCENE);
	gltfObject->extensionsAndExtras["LocalPath"] = strPath;;

	ptrScene->InitFromDocument(gltfObject);
	return ptrScene;
}

std::shared_ptr<fx::gltf::Document> BaseObject::OSG2GLTF(osg::ref_ptr<BaseObject> pObj)
{
	std::shared_ptr<fx::gltf::Document>	ptrDOC = std::make_shared<fx::gltf::Document>();





	return ptrDOC;
}

osg::ref_ptr<osg::PrimitiveSet> BaseObject::GetIndicesDrawElements(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive)
{
	assert(primitive.indices >= 0);
	osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(primitive.indices);
	if (pArr == nullptr)
	{
		return nullptr;
	}

	switch (pArr->getType())
	{
	case osg::Array::Type::ByteArrayType:
	case osg::Array::Type::UByteArrayType:
		return new osg::DrawElementsUByte(m_mapPrimitives[primitive.mode], pArr->getNumElements(), (const GLubyte*)(pArr->getDataPointer()));
	case osg::Array::Type::ShortArrayType:
	case osg::Array::Type::UShortArrayType:
		return new osg::DrawElementsUShort(m_mapPrimitives[primitive.mode], pArr->getNumElements(), (const GLushort*)(pArr->getDataPointer()));
	case osg::Array::Type::UIntArrayType:
	case osg::Array::Type::FloatArrayType:
		return new osg::DrawElementsUInt(m_mapPrimitives[primitive.mode], pArr->getNumElements(), (const GLuint*)(pArr->getDataPointer()));
	default:
		return nullptr;
	}
}

osg::ref_ptr<osg::PrimitiveSet> BaseObject::GetPrimitiveSet(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		return GetIndicesDrawElements(gltfObject, primitive);
	}
	else
	{
		return new osg::DrawArrays(m_mapPrimitives[primitive.mode]);
	}
}

void BaseObject::LoadTSRFromMatrix()
{
	vRotation = mat.getRotate().asVec4();
	vScale = mat.getScale();
	vTranslation = mat.getTrans();
}

void BaseObject::LoadAsset(const fx::gltf::Asset& asset)
{
	// TODO
	// Load order info and something else
}

void BaseObject::InitFromDocument(std::shared_ptr<fx::gltf::Document> gltfObject)
{
	SceneMgr::GetInstance().SetGLTF(gltfObject);

	LoadAsset(gltfObject->asset);
	// Scene level
	if (gltfObject.get() && gltfObject->scenes.size() > 0 && (gltfObject->scenes[0]).nodes.size() > 0)
	{
		const fx::gltf::Scene& scene = gltfObject->scenes[0];

		std::vector<int32_t>	nodes;
		for (auto id : scene.nodes)
		{
			nodes.push_back(static_cast<int32_t>(id));
		}
		AddNodes(gltfObject, nodes);
	}

	// TODO if necessary
	//std::vector<std::string> extensionsUsed{};
	//std::vector<std::string> extensionsRequired{};
}

void BaseObject::InitFromNode(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode)
{
	m_name = curNode.name;

	getOrCreateStateSet();

#pragma region LOAD_MATRIX
	if (ImportMatrix(curNode))
	{
		LoadTSRFromMatrix();
	}
	else
	{
		ImportRotation(curNode);
		ImportScale(curNode);
		ImportTranslation(curNode);
		RetsetMatrixFromTSR();
	}
	setMatrix(mat);
#pragma endregion LOAD_MATRIX

	ImportParams(curNode.extensionsAndExtras);
	ImportMesh(gltfObject, curNode);

	// TODO
	//int32_t camera{ -1 };
	//int32_t skin{ -1 };
	//std::vector<float> weights{};

	AddNodes(gltfObject, curNode.children);
}

void BaseObject::AddNodes(std::shared_ptr<fx::gltf::Document> gltfObject, const std::vector<int32_t>& nodes)
{
	for (int32_t childID : nodes)
	{
		const fx::gltf::Node& childNode = gltfObject->nodes[childID];
		osg::ref_ptr<BaseObject> pChild = ObjectFactory::CreateObject((ObjectFactory::CLASS_TYPE)GetClassType(childNode));
		if (pChild == nullptr)
		{
			continue;
		}
		pChild->InitFromNode(gltfObject, childNode);
		addChild(pChild);
	}
}

int BaseObject::GetClassType(const fx::gltf::Node& node)
{
	if (node.extensionsAndExtras.contains("class"))
	{
		return node.extensionsAndExtras["class"].get<int>();
	}
	return 0;
}

void BaseObject::RetsetMatrixFromTSR()
{
	mat.makeIdentity();
	mat.makeRotate(osg::Quat(vRotation));
	mat.makeScale(vScale);
	mat.makeTranslate(vTranslation);
}

bool BaseObject::ImportRotation(const fx::gltf::Node& node)
{
	vRotation.x() = node.rotation.at(0);
	vRotation.y() = node.rotation.at(1);
	vRotation.z() = node.rotation.at(2);
	vRotation.w() = node.rotation.at(3);
	return true;
}

bool BaseObject::ExportRotation(fx::gltf::Node& node)
{
	vRotation = mat.getRotate().asVec4();
	node.rotation.at(0) = vRotation.x();
	node.rotation.at(1) = vRotation.y();
	node.rotation.at(2) = vRotation.z();
	node.rotation.at(3) = vRotation.w();
	return true;
}

bool BaseObject::ImportScale(const fx::gltf::Node& node)
{
	vScale.x() = node.scale.at(0);
	vScale.y() = node.scale.at(1);
	vScale.z() = node.scale.at(2);
	return true;
}

bool BaseObject::ExportScale(fx::gltf::Node& node)
{
	vScale = mat.getScale();
	node.scale.at(0) = vScale.x();
	node.scale.at(1) = vScale.y();
	node.scale.at(2) = vScale.z();
	return true;
}

bool BaseObject::ImportTranslation(const fx::gltf::Node& node)
{
	vTranslation.x() = node.translation.at(0);
	vTranslation.y() = node.translation.at(1);
	vTranslation.z() = node.translation.at(2);
	return true;
}

bool BaseObject::ExportTranslation(fx::gltf::Node& node)
{
	vTranslation = mat.getTrans();
	node.translation.at(0) = vTranslation.x();
	node.translation.at(1) = vTranslation.y();
	node.translation.at(2) = vTranslation.z();
	return true;
}

bool BaseObject::ImportMatrix(const fx::gltf::Node& node)
{
	mat.set(node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3]
		, node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7]
		, node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11]
		, node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]);

	return mat.isIdentity() == false;
}

bool BaseObject::ExportMatrix(fx::gltf::Node& node)
{
	for (int ii = 0; ii < 16; ++ii)
	{
		node.matrix.at(ii) = *(mat.ptr() + ii);
	}

	return true;
}

bool BaseObject::ImportParams(const fx::gltf::Node& node)
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

bool BaseObject::ExportParams(fx::gltf::Node& node)
{
	auto paramArr = nlohmann::json::array();
	for (auto& kv : m_formulas)
	{
		auto obj = nlohmann::json::object();
		obj["name"] = kv.first;
		obj["formula"] = kv.second;
		paramArr.push_back(obj);
	}
	node.extensionsAndExtras["params"] = paramArr;
	return true;
}

bool BaseObject::ImportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node)
{
	if (node.mesh == -1)
	{
		return true;
	}

	const fx::gltf::Mesh& mesh = gltfObject->meshes[node.mesh];
	osg::ref_ptr<osg::Geode>	geo = new osg::Geode();
	addChild(geo);

	for (const fx::gltf::Primitive& primitive : mesh.primitives)
	{
		geo->addDrawable(ImportPrimitive(gltfObject, primitive));
	}

	return true;
}

bool BaseObject::ExportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node)
{
	return false;
}

osg::ref_ptr<osg::Drawable> BaseObject::ImportPrimitive(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive)
{
	osg::ref_ptr<osg::PrimitiveSet> ptrPrimitiveSet = GetPrimitiveSet(gltfObject, primitive);
	if (ptrPrimitiveSet == nullptr)
	{
		return nullptr;
	}

	getOrCreateStateSet();

	osg::ref_ptr<osg::Geometry>	ptrRet = new osg::Geometry();
	ptrRet->addPrimitiveSet(ptrPrimitiveSet);

	for (const std::pair<std::string, uint32_t>& attribute : primitive.attributes)
	{
		if (!(gltfObject->accessors.size() > attribute.second&& attribute.second >= 0))
		{
			continue;
		}

		if (attribute.first == "POSITION")
		{
			m_vertex = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (m_vertex)
			{
				ptrRet->setVertexArray(m_vertex);
			}
		}
		else if (attribute.first == "NORMAL")
		{
			m_normal = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (m_normal)
			{
				ptrRet->setNormalArray(m_normal, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "TANGENT")
		{
			osg::ref_ptr<osg::Array> pTangent = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pTangent)
			{
				// TODO
			}
		}
		else if (attribute.first == "TEXCOORD_0")
		{
			m_texCoord0 = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (m_texCoord0)
			{
				ptrRet->setTexCoordArray(0, m_texCoord0, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "TEXCOORD_1")
		{
			m_texCoord1 = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (m_texCoord1)
			{
				ptrRet->setTexCoordArray(1, m_texCoord1, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "JOINTS_0")
		{
			continue;
		}
		else if (attribute.first == "WEIGHTS_0")
		{
			continue;
		}
		else if (attribute.first == "COLOR_0")
		{
			continue;
		}
		else
		{
			assert(false);
		}
	}

	if (gltfObject->materials.size() > primitive.material&& primitive.material >= 0)
	{
		ImportMaterial(gltfObject, gltfObject->materials[primitive.material]);
	}

	// TODO 
	// std::vector<Attributes> targets{};

	return ptrRet;
}

bool BaseObject::ExportPrimitive(osg::ref_ptr<osg::Drawable> ptrDrawable, std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Primitive& primitive)
{
	return false;
}

bool BaseObject::ImportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material& material)
{
	
	bool bRet1 = LoadPBRTexture(gltfObject, material.pbrMetallicRoughness);
	bool bRet2 = LoadNormalTexture(gltfObject, material.normalTexture);
	bool bRet3 = LoadOcclusionTexture(gltfObject, material.occlusionTexture);
	bool bRet4 = LoadImageTexture(gltfObject, material.emissiveTexture);

	// TODO
	//float alphaCutoff{ defaults::MaterialAlphaCutoff };
	//AlphaMode alphaMode{ AlphaMode::Opaque };

	//bool doubleSided{ defaults::MaterialDoubleSided };

	//Texture emissiveTexture;
	//std::array<float, 3> emissiveFactor = { defaults::NullVec3 };

	return bRet1 || bRet2 || bRet3 || bRet4;
}

bool BaseObject::ExportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Material& Material)
{
	Material.normalTexture.empty();
	return false;
}

bool BaseObject::ParseParams(const nlohmann::json::value_type& params)
{
	if (!params.is_array())
	{
		LOG(ERROR) << "params is not array";
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

bool BaseObject::LoadColorTexture(std::shared_ptr<fx::gltf::Document> gltfObject, const std::array<float, 4>& baseColorFactor)
{
	osg::Uniform* baseTextureSampler = new osg::Uniform("baseColorFactor", osg::Vec4(baseColorFactor[0], baseColorFactor[1], baseColorFactor[2], baseColorFactor[3]));
	osg::Uniform* useBaseColorFactor = new osg::Uniform("useBaseColorFactor", fx::gltf::defaults::IdentityVec4 != baseColorFactor ? 1 : 0);
	getOrCreateStateSet()->addUniform(useBaseColorFactor);
	getOrCreateStateSet()->addUniform(baseTextureSampler);

	return fx::gltf::defaults::IdentityVec4 != baseColorFactor;
}

bool BaseObject::LoadImageTexture(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material::Texture& texture)
{
	if (texture.empty())
	{
		return false;
	}
	osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(texture.index);
	if (pTexture)
	{
		getOrCreateStateSet()->setTextureAttributeAndModes(texture.texCoord, pTexture);
		osg::Uniform* baseTextureSampler = new osg::Uniform("baseTexture", texture.texCoord);
		getOrCreateStateSet()->addUniform(baseTextureSampler);
		return true;
	}
	return false;
}

bool BaseObject::LoadPBRTexture(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material::PBRMetallicRoughness& pbrMaterial)
{
	if (pbrMaterial.empty())
	{
		return false;
	}

	bool bRet1 = LoadColorTexture(gltfObject, pbrMaterial.baseColorFactor);
	bool bRet2 = LoadImageTexture(gltfObject, pbrMaterial.baseColorTexture);
	bool bRet3 = LoadImageTexture(gltfObject, pbrMaterial.metallicRoughnessTexture);

	// TODO
	//float roughnessFactor{ defaults::IdentityScalar };
	//float metallicFactor{ defaults::IdentityScalar };

	return bRet1 || bRet2 || bRet3;
}

bool BaseObject::LoadNormalTexture(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material::NormalTexture& normalTexture)
{
	auto fn = [&]()
	{
		return false;
	};

	bool bRet = LoadImageTexture(gltfObject, normalTexture) && fn();

	// TODO
	// float scale{ defaults::IdentityScalar };

	return bRet;
}

bool BaseObject::LoadOcclusionTexture(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material::OcclusionTexture& occlusionTexture)
{
	auto fn = [&]()
	{
		return false;
	};

	bool bRet = LoadImageTexture(gltfObject, occlusionTexture) && fn();

	// TODO
	// float strength{ defaults::IdentityScalar };

	return bRet;
}
