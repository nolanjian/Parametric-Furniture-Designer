#include "pch.h"
#include "BaseObject.h"
#include "../Utils/IDGenerater.h"
#include "../Utils/IParamsConverter.h"
#include "../easyloggingpp/easylogging++.h"
#include "ObjectFactory.h"

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
		std::shared_ptr<fx::gltf::Document>	ptrObj = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(ss, std::filesystem::current_path().u8string()));
		return GLTF2OSG(ptrObj);
	}
	catch (const std::exception& e)
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
		std::shared_ptr<fx::gltf::Document>	ptrDOC = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(strPath));
		return GLTF2OSG(ptrDOC);
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

osg::ref_ptr<BaseObject> BaseObject::GLTF2OSG(std::shared_ptr<fx::gltf::Document> gltfObject)
{
	osg::ref_ptr<BaseObject> ptrScene = ObjectFactory::CreateObject(ObjectFactory::SCENE);
	ptrScene->InitFromDocument(gltfObject);
	return ptrScene;
}

std::shared_ptr<fx::gltf::Document> BaseObject::OSG2GLTF(osg::ref_ptr<BaseObject> pObj)
{
	std::shared_ptr<fx::gltf::Document>	ptrDOC = std::make_shared<fx::gltf::Document>();





	return ptrDOC;
}

std::shared_ptr<RawBufferInfo> BaseObject::GetRawBufferInfo(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Accessor& accessor)
{
	std::shared_ptr<RawBufferInfo> pInfo = std::make_shared<RawBufferInfo>();

	fx::gltf::BufferView const& bufferView = gltfObject->bufferViews[accessor.bufferView];
	fx::gltf::Buffer const& buffer = gltfObject->buffers[bufferView.buffer];

	const uint32_t dataTypeSize = GetElementSize(accessor);
	pInfo->accessor = &accessor;
	pInfo->data = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset];
	pInfo->dataStride = dataTypeSize;
	pInfo->totalSize = accessor.count * dataTypeSize;
	return pInfo;
}

osg::ref_ptr<osg::Vec4dArray> BaseObject::GetVec4Array(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Accessor& accessor)
{
	size_t itemSize = 0;
	switch (accessor.type)
	{
	case fx::gltf::Accessor::Type::Vec2:
		itemSize = 2;
		break;
	case fx::gltf::Accessor::Type::Vec3:
		itemSize = 3;
		break;
	case fx::gltf::Accessor::Type::Vec4:
		itemSize = 4;
		break;
	default:
		LOG(ERROR) << "GetVec4Array only support Vec2,Vec3,Vec4";
		return nullptr;
	}

	size_t	elementSize = 0;
	switch (accessor.componentType)
	{
	case fx::gltf::Accessor::ComponentType::Byte:
	case fx::gltf::Accessor::ComponentType::UnsignedByte:
		elementSize = 1;
		break;
	case fx::gltf::Accessor::ComponentType::Short:
	case fx::gltf::Accessor::ComponentType::UnsignedShort:
		elementSize = 2;
		break;
	case fx::gltf::Accessor::ComponentType::UnsignedInt:
	case fx::gltf::Accessor::ComponentType::Float:
		elementSize = 4;
		break;
	default:
		return nullptr;
	}

	fx::gltf::BufferView const& bufferView = gltfObject->bufferViews[accessor.bufferView];
	fx::gltf::Buffer const& buffer = gltfObject->buffers[bufferView.buffer];

	auto bufOffset = static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset;
	
	const uint8_t* pData = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset];

	size_t totalSize = itemSize * elementSize * accessor.count;
	uint8_t* pBuf = (uint8_t*)malloc(totalSize * sizeof(uint8_t));
	memcpy(pBuf, pData, totalSize * sizeof(uint8_t));

	osg::ref_ptr<osg::Vec4dArray> arr = new osg::Vec4dArray();
	for (size_t ii = 0; ii < accessor.count; ++ii)
	{
		osg::Vec4d	vec(0, 0, 0, 1);
		const uint8_t* pCurItem = pBuf + itemSize * elementSize * ii;
		for (size_t jj = 0; jj < itemSize; ++jj)
		{
			const uint8_t* pCurElement = pCurItem + jj * elementSize;
			
			switch (accessor.componentType)
			{
			case fx::gltf::Accessor::ComponentType::Byte:
			{
				int8_t nVal = *((int8_t*)(pCurElement));
				vec[jj] = double(nVal);
				break;
			}
			case fx::gltf::Accessor::ComponentType::UnsignedByte:
			{
				uint8_t nVal = *((uint8_t*)(pCurElement));
				vec[jj] = double(nVal);
				break;
			}
			case fx::gltf::Accessor::ComponentType::Short:
			{
				int16_t nVal = *((int16_t*)(pCurElement));
				vec[jj] = double(nVal);
				break;
			}
			case fx::gltf::Accessor::ComponentType::UnsignedShort:
			{
				uint16_t nVal = *((uint16_t*)(pCurElement));
				vec[jj] = double(nVal);
				break;
			}
			case fx::gltf::Accessor::ComponentType::UnsignedInt:
			{
				uint32_t nVal = *((uint32_t*)(pCurElement));
				vec[jj] = double(nVal);
				break;
			}
			case fx::gltf::Accessor::ComponentType::Float:
			{
				float nVal = *((float*)(pCurElement));
				vec[jj] = double(nVal);
				break;
			}
			default:
				return nullptr;
			}


		}
		arr->push_back(vec);
	}
	assert(accessor.count == arr->size());
	return arr;
}

std::vector<double> BaseObject::GetNumArray(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Accessor& accessor)
{
	size_t itemSize = 0;
	switch (accessor.type)
	{
	case fx::gltf::Accessor::Type::Scalar:
		itemSize = 1;
		break;
	default:
		LOG(ERROR) << "GetNumArray only support Scalar";
		return std::vector<double>();
	}

	size_t	elementSize = 0;
	switch (accessor.componentType)
	{
	case fx::gltf::Accessor::ComponentType::Byte:
	case fx::gltf::Accessor::ComponentType::UnsignedByte:
		elementSize = 1;
		break;
	case fx::gltf::Accessor::ComponentType::Short:
	case fx::gltf::Accessor::ComponentType::UnsignedShort:
		elementSize = 2;
		break;
	case fx::gltf::Accessor::ComponentType::UnsignedInt:
	case fx::gltf::Accessor::ComponentType::Float:
		elementSize = 4;
		break;
	default:
		return std::vector<double>();
	}

	fx::gltf::BufferView const& bufferView = gltfObject->bufferViews[accessor.bufferView];
	fx::gltf::Buffer const& buffer = gltfObject->buffers[bufferView.buffer];

	auto bufOffset = static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset;

	const uint8_t* pData = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset];

	size_t totalSize = itemSize * elementSize * accessor.count;
	uint8_t* pBuf = (uint8_t*)malloc(totalSize * sizeof(uint8_t));
	memcpy(pBuf, pData, totalSize * sizeof(uint8_t));

	std::vector<double>	arr;
	for (size_t ii = 0; ii < accessor.count; ++ii)
	{
		const uint8_t* pCurItem = pBuf + itemSize * elementSize * ii;
		for (size_t jj = 0; jj < itemSize; ++jj)
		{
			const uint8_t* pCurElement = pCurItem + jj * elementSize;

			switch (accessor.componentType)
			{
			case fx::gltf::Accessor::ComponentType::Byte:
			{
				int8_t nVal = *((int8_t*)(pCurElement));
				arr.push_back(double(nVal));
				break;
			}
			case fx::gltf::Accessor::ComponentType::UnsignedByte:
			{
				uint8_t nVal = *((uint8_t*)(pCurElement));
				arr.push_back(double(nVal));
				break;
			}
			case fx::gltf::Accessor::ComponentType::Short:
			{
				int16_t nVal = *((int16_t*)(pCurElement));
				arr.push_back(double(nVal));
				break;
			}
			case fx::gltf::Accessor::ComponentType::UnsignedShort:
			{
				uint16_t nVal = *((uint16_t*)(pCurElement));
				arr.push_back(double(nVal));
				break;
			}
			case fx::gltf::Accessor::ComponentType::UnsignedInt:
			{
				uint32_t nVal = *((uint32_t*)(pCurElement));
				arr.push_back(double(nVal));
				break;
			}
			case fx::gltf::Accessor::ComponentType::Float:
			{
				float nVal = *((float*)(pCurElement));
				arr.push_back(double(nVal));
				break;
			}
			}
		}
	}
	return arr;
}

void BaseObject::LoadTSRFromMatrix()
{
	vRotation = mat.getRotate().asVec4();
	vScale = mat.getScale();
	vTranslation = mat.getTrans();
}

void BaseObject::InitFromDocument(std::shared_ptr<fx::gltf::Document> gltfObject)
{
	// Scene level
	if (gltfObject.get() && gltfObject->scenes.size() > 0 && (gltfObject->scenes[0]).nodes.size() > 0)
	{
		const fx::gltf::Node& curNode = gltfObject->nodes[gltfObject->scenes[0].nodes[0]];
		InitFromNode(gltfObject, curNode);
	}
}

void BaseObject::InitFromNode(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode)
{
	m_name = curNode.name;
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

#pragma region UPDATE_CHILDREN
	for (auto childID : curNode.children)
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
#pragma endregion UPDATE_CHILDREN
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
#pragma region LOAD_ATTRIBUTE
	for (const std::pair<std::string, uint32_t>& attribute : primitive.attributes)
	{
		if (attribute.first == "POSITION")
		{
			m_vertex = GetVec4Array(gltfObject, gltfObject->accessors[attribute.second]);
		}
		else if (attribute.first == "NORMAL")
		{
			m_normal = GetVec4Array(gltfObject, gltfObject->accessors[attribute.second]);
		}
		else if (attribute.first == "TANGENT")
		{
			//m_tangentBuffer = GetRawBufferInfo(gltfObject, gltfObject->accessors[attribute.second]);
		}
		else if (attribute.first == "TEXCOORD_0")
		{
			//m_texCoord0Buffer = GetRawBufferInfo(gltfObject, gltfObject->accessors[attribute.second]);
		}
	}
#pragma endregion LOAD_ATTRIBUTE
	m_indexl = GetNumArray(gltfObject, gltfObject->accessors[primitive.indices]);
#pragma region ATTRIBUTE_TO_OSG_OBJECT
	// TODO
#pragma endregion ATTRIBUTE_TO_OSG_OBJECT

	if (primitive.material >= 0)
	{
		//TODO LOAD MATRERIAL
	}

	auto fnGetOSGPrimitiveType = [&]()
	{
		switch (primitive.mode)
		{
		case fx::gltf::Primitive::Mode::Points:
			return osg::PrimitiveSet::POINTS;
		case fx::gltf::Primitive::Mode::Lines:
			return osg::PrimitiveSet::LINES;
		case fx::gltf::Primitive::Mode::LineLoop:
			return osg::PrimitiveSet::LINE_LOOP;
		case fx::gltf::Primitive::Mode::LineStrip:
			return osg::PrimitiveSet::LINE_STRIP;
		case fx::gltf::Primitive::Mode::Triangles:
			return osg::PrimitiveSet::TRIANGLES;
		case fx::gltf::Primitive::Mode::TriangleStrip:
			return osg::PrimitiveSet::TRIANGLE_STRIP;
		case fx::gltf::Primitive::Mode::TriangleFan:
			return osg::PrimitiveSet::TRIANGLE_FAN;
		default:
			return osg::PrimitiveSet::POINTS;
		}
	};

	osg::ref_ptr<osg::Geometry>	ptrRet = new osg::Geometry();

	auto ptrPrimitiveSet = new osg::DrawElementsUInt(fnGetOSGPrimitiveType());
	for (auto index : m_indexl)
	{
		ptrPrimitiveSet->push_back(unsigned int(index));
	}

	ptrRet->addPrimitiveSet(ptrPrimitiveSet);
	ptrRet->setVertexArray(m_vertex);
	ptrRet->setNormalArray(m_normal);

	osg::Vec4dArray* pColor = new osg::Vec4dArray();
	pColor->push_back(osg::Vec4(255, 0, 0, 255));

	ptrRet->setColorArray(pColor, osg::Array::Binding::BIND_OVERALL);
	if (m_texCoord0)
	{
		ptrRet->setTexCoordArray(0, m_texCoord0);
	}
	if (m_texCoord1)
	{
		ptrRet->setTexCoordArray(1, m_texCoord1);
	}
	if (m_texCoord2)
	{
		ptrRet->setTexCoordArray(2, m_texCoord2);
	}

	return ptrRet;
}

bool BaseObject::ExportPrimitive(osg::ref_ptr<osg::Drawable> ptrDrawable, std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Primitive& primitive)
{
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

uint32_t BaseObject::GetElementSize(const fx::gltf::Accessor& accessor)
{
	uint32_t elementSize = 0;
	switch (accessor.componentType)
	{
	case fx::gltf::Accessor::ComponentType::Byte:
	case fx::gltf::Accessor::ComponentType::UnsignedByte:
		elementSize = 1;
		break;
	case fx::gltf::Accessor::ComponentType::Short:
	case fx::gltf::Accessor::ComponentType::UnsignedShort:
		elementSize = 2;
		break;
	case fx::gltf::Accessor::ComponentType::UnsignedInt:
	case fx::gltf::Accessor::ComponentType::Float:
		elementSize = 4;
		break;
	default:
		break;
	}

	switch (accessor.type)
	{
	case fx::gltf::Accessor::Type::Scalar:
		return elementSize;
	case fx::gltf::Accessor::Type::Vec2:
		return elementSize * 2;
	case fx::gltf::Accessor::Type::Vec3:
		return elementSize * 3;
	case fx::gltf::Accessor::Type::Vec4:
		return elementSize * 4;
	case fx::gltf::Accessor::Type::Mat2:
		return elementSize * 4;
	case fx::gltf::Accessor::Type::Mat3:
		return elementSize * 9;
	case fx::gltf::Accessor::Type::Mat4:
		return elementSize * 16;
	default:
		break;
	}

	return 0;
}
