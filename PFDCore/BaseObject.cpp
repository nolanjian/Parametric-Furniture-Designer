#include "pch.h"
#include "BaseObject.h"
#include "../Utils/IDGenerater.h"
#include "../Utils/IParamsConverter.h"
#include "ObjectFactory.h"
#include "SceneMgr.h"
#include "Coordinate.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

namespace PFDCore
{

	BaseObject::BaseObject()
		:osg::MatrixTransform()
	{
		m_ID = PFDUtils::IDGenerater::Get()->GetNext();
	}

	BaseObject::~BaseObject()
	{
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
		setName(curNode.name);

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

		//ImportParams(curNode.extensionsAndExtras);
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

	osg::BoundingBox BaseObject::GetBoundingBox()
	{
		osg::ComputeBoundsVisitor boundVisitor;
		accept(boundVisitor);
		return boundVisitor.getBoundingBox();
	}

	osg::BoundingSphere BaseObject::GetBoundingSphere()
	{
		dirtyBound();
		return getBound();
	}

	void BaseObject::RetsetMatrixFromTSR()
	{
		mat.makeIdentity();
		mat.makeRotate(osg::Quat(vRotation));
		mat.makeScale(vScale);
		mat.makeTranslate(vTranslation);
	}

	bool BaseObject::SetPosition(const osg::Vec3& pos)
	{
		vTranslation = pos;
		RetsetMatrixFromTSR();
		setMatrix(mat);
		return true;
	}

	osg::Vec3 BaseObject::GetPosition()
	{
		return vTranslation;
	}

	bool BaseObject::UpdatePrimitive()
	{
		for (auto& child : _children)
		{
			BaseObject* bo = dynamic_cast<BaseObject*>(child.get());
			if (bo)
			{
				bo->UpdatePrimitive();
			}
		}
		return true;
	}

	void BaseObject::ShowCoordinate(bool bShow)
	{
		std::list<osg::Node*>	lsCoordinate;
		for (osg::Node* pNode : _children)
		{
			if (pNode->getName() == "Coordinate")
			{
				lsCoordinate.push_back(pNode);
			}
		}
		for (osg::Node* pNode : lsCoordinate)
		{
			removeChild(pNode);
		}

		if (bShow)
		{
			addChild(new Coordinate());
		}
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

	//bool BaseObject::ExportParams(fx::gltf::Node& node)
	//{
	//	auto paramArr = nlohmann::json::array();
	//	for (auto& kv : m_formulas)
	//	{
	//		auto obj = nlohmann::json::object();
	//		obj["name"] = kv.first;
	//		obj["formula"] = kv.second;
	//		paramArr.push_back(obj);
	//	}
	//	node.extensionsAndExtras["params"] = paramArr;
	//	return true;
	//}

	bool BaseObject::ImportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node)
	{
		if (node.mesh == -1)
		{
			return true;
		}

		m_geo = SceneMgr::GetInstance().GetMeshManager().GetInstance().GetMesh(node.mesh);
		if (m_geo)
		{
			addChild(m_geo);
		}
		return true;
	}

	bool BaseObject::ExportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node)
	{
		return false;
	}

	bool BaseObject::ExportPrimitive(osg::ref_ptr<osg::Drawable> ptrDrawable, std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Primitive& primitive)
	{
		return false;
	}

	bool BaseObject::ExportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Material& Material)
	{
		Material.normalTexture.empty();
		return false;
	}

}