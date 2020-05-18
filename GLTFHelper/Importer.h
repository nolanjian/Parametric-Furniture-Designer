#pragma once
#include "GLTFHelperExport.h"
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "fx/gltf.h"
#include "IComponentFactory.h"
#include "BaseObject.h"
#include "IGLTFResourceManager.h"

namespace PFD_GLTF_Helper
{
	// JUST Load, do not do params update
	class PFD_GLTF_HELPER_API Importer
	{
	public:
		Importer();
		~Importer();

		osg::ref_ptr<osg::Group> Load(const std::wstring& path);
		osg::ref_ptr<osg::Group> Load(const std::string& utf8Json);
		osg::ref_ptr<osg::Group> Load(std::shared_ptr<fx::gltf::Document> gltf);

	protected:
		void LoadAsset(const fx::gltf::Asset& asset);
		void AddNodes(osg::ref_ptr<PFDComponent::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const std::vector<int32_t>& nodes);
		PFDComponent::IComponentFactory::Type GetClassType(const fx::gltf::Node& node);

		osg::Matrix	GetMatrix(const fx::gltf::Node& curNode);
		osg::Matrix	GetMatrix(const osg::Vec3& tran, const osg::Vec3& scale, const osg::Quat& quat);
		osg::Quat	GetRotation(const fx::gltf::Node& node);
		osg::Vec3	GetScale(const fx::gltf::Node& node);
		osg::Vec3	GetTran(const fx::gltf::Node& node);

		void InitFromGLTFNode(osg::ref_ptr<PFDComponent::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode);
		bool ImportMesh(osg::ref_ptr<PFDComponent::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode);
	private:
		std::shared_ptr<IGLTFResourceManager>	m_pResourceManager;
		std::shared_ptr<fx::gltf::Document> m_pDoc;
		std::string m_utf8Asset;
	};
}