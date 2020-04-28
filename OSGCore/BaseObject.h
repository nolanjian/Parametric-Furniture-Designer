#pragma once

#include "OSGCoreExport.h"
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include <fx/gltf.h>
#include "../easyloggingpp/easylogging++.h"
#include "../muparserx/mpParser.h"
#include "../muparserx/mpDefines.h"
#include "../muparserx/mpTypes.h"

namespace PFDCore
{
	class OSGCORE_API BaseObject : public osg::MatrixTransform
	{
	public:
		BaseObject();
		virtual ~BaseObject();

		// GLTF JSON STRING
		static osg::ref_ptr<BaseObject> JSON2OSG(const std::string& str);
		static std::string OSG2JSON(osg::ref_ptr<BaseObject> pObj);
		// GLTF MEMORY MODEL OBJECT
		static osg::ref_ptr<Group>	LoadSceneFromJsonFile(const std::string& strPath);
		static osg::ref_ptr<BaseObject> GLTF2OSG(std::shared_ptr<fx::gltf::Document> gltfObject, const std::string& strPath);
		static std::shared_ptr<fx::gltf::Document> OSG2GLTF(osg::ref_ptr<BaseObject> pObj);

		void	LoadTSRFromMatrix();
		void	LoadAsset(const fx::gltf::Asset& asset);
		void	InitFromDocument(std::shared_ptr<fx::gltf::Document> gltfObject);
		void	InitFromNode(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& curNode);
		void	AddNodes(std::shared_ptr<fx::gltf::Document> gltfObject, const std::vector<int32_t>& nodes);
		int		GetClassType(const fx::gltf::Node& node);

		osg::BoundingBox GetBoundingBox();
		osg::BoundingSphere GetBoundingSphere();

		void	RetsetMatrixFromTSR();

		bool		SetPosition(const osg::Vec3& pos);
		osg::Vec3	GetPosition();

		virtual bool UpdatePrimitive();

		std::string	m_name;
		std::string	m_gltfPath;

		void	ShowCoordinate(bool bShow);

	protected:
		bool ImportRotation(const fx::gltf::Node& node);
		bool ExportRotation(fx::gltf::Node& node);
		bool ImportScale(const fx::gltf::Node& node);
		bool ExportScale(fx::gltf::Node& node);
		bool ImportTranslation(const fx::gltf::Node& node);
		bool ExportTranslation(fx::gltf::Node& node);
		bool ImportMatrix(const fx::gltf::Node& node);
		bool ExportMatrix(fx::gltf::Node& node);
		//bool ImportParams(const fx::gltf::Node& node);
		//bool ExportParams(fx::gltf::Node& node);
		bool ImportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node);
		bool ExportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node);
		//osg::ref_ptr<osg::Drawable> ImportPrimitive(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive);
		bool ExportPrimitive(osg::ref_ptr<osg::Drawable> ptrDrawable, std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Primitive& primitive);
		//bool ImportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material& Material);
		bool ExportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Material& Material);


		osg::Vec4	vRotation;
		osg::Vec3	vScale;
		osg::Vec3	vTranslation;
		osg::Matrixd	mat;

		osg::ref_ptr<osg::Geode> m_geo;

	private:
		bool m_bEnableParams = false;	// TODO, if get not any params from gltf, just let it load and disable params changes.
		unsigned long long int	m_ID = 0;
		bool m_bDirty = true;
		osg::ref_ptr<BaseObject>	m_parent;

		osg::ref_ptr<osg::Array>	m_vertex;
		osg::ref_ptr<osg::Array>	m_normal;
		osg::ref_ptr<osg::Array>	m_indexl;
		osg::ref_ptr<osg::Array>	m_texCoord0;
		osg::ref_ptr<osg::Array>	m_texCoord1;
	};
};