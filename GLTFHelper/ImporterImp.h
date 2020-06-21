/*****************************************************************//**
 * \file   ImporterImp.h
 * \brief  GLTF Importer Implement
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/09
 *********************************************************************/
#pragma once

#include "Importer.h"
#include <Component/IComponentFactory.h>
#include <Component/BaseObject.h>
#include "IGLTFResourceManager.h"

namespace PFD
{
	namespace GLTF
	{
		/**
		 * \brief  GLTF Importer Class Define
		 * \author NolanJian
		 * \e-mail NolanJian@163.com
		 * \date   2020/06/09
		 * 
		 * \WARN   Just do the import job, don`t make any pm update, do update outside
		 * */
		class ImporterImp : public Importer
		{
		public:
			/// Interface Implement Begin
			virtual ~ImporterImp();
			virtual osg::ref_ptr<osg::Group> Load(const std::wstring& path) override;
			virtual osg::ref_ptr<osg::Group> Load(const std::string& utf8Json) override;
			virtual osg::ref_ptr<osg::Group> Load(std::shared_ptr<fx::gltf::Document> gltf) override;
			virtual std::shared_ptr<fx::gltf::Document> GetGLTF() override;
			/// Interface Implement End
		public:
			ImporterImp();
		protected:
			void LoadAsset(const fx::gltf::Asset& asset);
			void AddNodes(osg::ref_ptr<PFD::Component::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const std::vector<int32_t>& nodes);
			PFD::Component::IComponentFactory::Type GetClassType(const fx::gltf::Node& node);

			osg::Matrix	GetMatrix(const fx::gltf::Node& curNode);
			osg::Matrix	GetMatrix(const osg::Vec3& tran, const osg::Vec3& scale, const osg::Quat& quat);
			osg::Quat	GetRotation(const fx::gltf::Node& node);
			osg::Vec3	GetScale(const fx::gltf::Node& node);
			osg::Vec3	GetTran(const fx::gltf::Node& node);

			void InitFromGLTFNode(osg::ref_ptr<PFD::Component::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode);
			bool ImportMesh(osg::ref_ptr<PFD::Component::BaseObject> pNode, std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode);
		private:
			std::shared_ptr<IGLTFResourceManager>	m_pResourceManager;
			std::shared_ptr<fx::gltf::Document> m_pDoc;
			std::string m_utf8Asset;
		};
	}
}