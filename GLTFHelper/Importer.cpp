#include "Importer.h"
#include "Utils.h"

namespace PFD
{
	namespace GLTF
	{

		Importer::Importer()
		{
		}

		Importer::~Importer()
		{
		}

		osg::ref_ptr<osg::Group> Importer::Load(const std::wstring& path)
		{
			std::string strPath = PFD::Utils::WStringToString(path);
			if (strPath.find(".glb") == std::string::npos)
			{
				m_pDoc = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(strPath));
			}
			else
			{
				m_pDoc = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromBinary(strPath));
			}
			m_pDoc->extensionsAndExtras["LocalPath"] = strPath;
			return Load(m_pDoc);
		}

		osg::ref_ptr<osg::Group> Importer::Load(const std::string& utf8Json)
		{
			std::stringstream	ss;
			ss << utf8Json;
			m_pDoc = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(ss, fx::gltf::detail::GetDocumentRootPath(utf8Json)));
			m_pDoc->extensionsAndExtras["LocalPath"] = std::filesystem::current_path().string();

			return Load(m_pDoc);
		}

		osg::ref_ptr<osg::Group> Importer::Load(std::shared_ptr<fx::gltf::Document> gltf)
		{
			if (!m_pDoc)
			{
				return nullptr;
			}

			m_pDoc = gltf;
			LoadAsset(gltf->asset);

			m_pResourceManager = IGLTFResourceManager::Load(m_pDoc);



			osg::ref_ptr<PFD::Component::BaseObject> pScene = new PFD::Component::BaseObject();

			// Scene level
			if (m_pDoc->scenes.size() > 0 && (m_pDoc->scenes[0]).nodes.size() > 0)
			{
				const fx::gltf::Scene& scene = m_pDoc->scenes[0];

				std::vector<int32_t>	nodes;
				for (auto id : scene.nodes)
				{
					nodes.push_back(static_cast<int32_t>(id));
				}
				AddNodes(pScene, m_pDoc, nodes);
			}

			// TODO if necessary
			//std::vector<std::string> extensionsUsed{};
			//std::vector<std::string> extensionsRequired{};

			return pScene;
		}

		void Importer::LoadAsset(const fx::gltf::Asset& asset)
		{
			// TODO
		}

		void Importer::AddNodes(osg::ref_ptr<PFD::Component::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const std::vector<int32_t>& nodes)
		{
			for (int32_t childID : nodes)
			{
				const fx::gltf::Node& childNode = gltfObject->nodes[childID];

				osg::ref_ptr<PFD::Component::BaseObject> pChild = PFD::Component::IComponentFactory::Get()->CreateObject(GetClassType(childNode));
				if (pChild == nullptr)
				{
					continue;
				}
				InitFromGLTFNode(pChild, gltfObject, childNode);

				pNode->addChild(pChild);
			}
		}

		PFD::Component::IComponentFactory::Type Importer::GetClassType(const fx::gltf::Node& node)
		{
			if (node.extensionsAndExtras.contains("class"))
			{
				return (PFD::Component::IComponentFactory::Type)node.extensionsAndExtras["class"].get<int>();
			}

			return PFD::Component::IComponentFactory::Type::BASE_OBJECT;
		}

		osg::Matrix Importer::GetMatrix(const fx::gltf::Node& curNode)
		{
			osg::Matrix mat;
			mat.set(curNode.matrix[0], curNode.matrix[1], curNode.matrix[2], curNode.matrix[3]
				, curNode.matrix[4], curNode.matrix[5], curNode.matrix[6], curNode.matrix[7]
				, curNode.matrix[8], curNode.matrix[9], curNode.matrix[10], curNode.matrix[11]
				, curNode.matrix[12], curNode.matrix[13], curNode.matrix[14], curNode.matrix[15]);
			return mat;
		}

		osg::Matrix Importer::GetMatrix(const osg::Vec3& tran, const osg::Vec3& scale, const osg::Quat& quat)
		{
			osg::Matrix mat;

			mat.postMultRotate(quat);
			mat.postMultTranslate(tran);
			mat.postMultScale(scale);
			return mat;
		}

		osg::Quat Importer::GetRotation(const fx::gltf::Node& node)
		{
			osg::Quat quat;
			quat.x() = node.rotation.at(0);
			quat.y() = node.rotation.at(1);
			quat.z() = node.rotation.at(2);
			quat.w() = node.rotation.at(3);
			return quat;
		}

		osg::Vec3 Importer::GetScale(const fx::gltf::Node& node)
		{
			osg::Vec3 scale;
			scale.x() = node.scale.at(0);
			scale.y() = node.scale.at(1);
			scale.z() = node.scale.at(2);
			return scale;
		}

		osg::Vec3 Importer::GetTran(const fx::gltf::Node& node)
		{
			osg::Vec3 tran;
			tran.x() = node.translation.at(0);
			tran.y() = node.translation.at(1);
			tran.z() = node.translation.at(2);
			return tran;
		}

		void Importer::InitFromGLTFNode(osg::ref_ptr<PFD::Component::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode)
		{
			pNode->setName(curNode.name);
			pNode->getOrCreateStateSet();

#pragma region LOAD_MATRIX
			osg::Matrix mat = GetMatrix(curNode);
			if (mat.isIdentity())
			{
				osg::Quat quat = GetRotation(curNode);
				osg::Vec3 scale = GetScale(curNode);
				osg::Vec3 tran = GetTran(curNode);

				mat = GetMatrix(tran, scale, quat);
			}
			pNode->setMatrix(mat);
#pragma endregion LOAD_MATRIX

			//ImportParams(curNode.extensionsAndExtras);
			ImportMesh(pNode, gltfObject, curNode);

			// TODO
			//int32_t camera{ -1 };
			//int32_t skin{ -1 };
			//std::vector<float> weights{};

			AddNodes(pNode, gltfObject, curNode.children);
		}

		bool Importer::ImportMesh(osg::ref_ptr<PFD::Component::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode)
		{
			if (curNode.mesh == -1)
			{
				return true;
			}

			osg::ref_ptr<osg::Geode> geo = m_pResourceManager->GetMesh(curNode.mesh);
			if (geo)
			{
				pNode->addChild(geo);
			}
			return true;
		}


	}
}