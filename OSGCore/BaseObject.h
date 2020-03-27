#pragma once



#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "../muparserx/mpParser.h"
#include "../muparserx/mpDefines.h"
#include "../muparserx/mpTypes.h"

#include <fx/gltf.h>

class BaseObject : public osg::MatrixTransform
{
public:
	BaseObject();
	virtual ~BaseObject();

	virtual bool InitFromParams() { return false; };

	void SetParent(osg::ref_ptr<BaseObject> parent) { m_parent = parent; }
	osg::ref_ptr<BaseObject> GetParent() { return m_parent; }

	const std::map<std::wstring, std::wstring>& GetFormulas() { return m_formulas; }

	bool ReInitParser();
	bool SetParentFormulars();
	bool UpdateFormulas();
	bool UpdateSelfFormulas();
	bool SetOneLine(const std::wstring& line);

	const mup::var_maptype& FormulasResult();

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

	void	RetsetMatrixFromTSR();

	std::string	m_name;
	std::string	m_gltfPath;

protected:
	bool ImportRotation(const fx::gltf::Node& node);
	bool ExportRotation(fx::gltf::Node& node);
	bool ImportScale(const fx::gltf::Node& node);
	bool ExportScale(fx::gltf::Node& node);
	bool ImportTranslation(const fx::gltf::Node& node);
	bool ExportTranslation(fx::gltf::Node& node);
	bool ImportMatrix(const fx::gltf::Node& node);
	bool ExportMatrix(fx::gltf::Node& node);
	bool ImportParams(const fx::gltf::Node& node);
	bool ExportParams(fx::gltf::Node& node);
	bool ImportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node);
	bool ExportMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node);
	//osg::ref_ptr<osg::Drawable> ImportPrimitive(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive);
	bool ExportPrimitive(osg::ref_ptr<osg::Drawable> ptrDrawable, std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Primitive& primitive);
	//bool ImportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Material& Material);
	bool ExportMaterial(std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Material& Material);
	bool ParseParams(const nlohmann::json::value_type& params);

	osg::Vec4	vRotation;
	osg::Vec3	vScale;
	osg::Vec3	vTranslation;
	osg::Matrixd	mat;

	std::map<std::wstring, std::wstring>	m_formulas;
	std::map<std::wstring, std::wstring>	m_formulasResult4Cal;
	std::map<std::wstring, std::wstring>	m_parentFormulasResult;

	mup::var_maptype	m_formulasResult;

private:
	bool m_bEnableParams = false;	// TODO, if get not any params from gltf, just let it load and disable params changes.
	unsigned long long int	m_ID = 0;
	bool m_bDirty = true;
	osg::ref_ptr<BaseObject>	m_parent;
	std::shared_ptr<mup::ParserX>	m_parser;

	osg::ref_ptr<osg::Array>	m_vertex;
	osg::ref_ptr<osg::Array>	m_normal;
	osg::ref_ptr<osg::Array>	m_indexl;
	osg::ref_ptr<osg::Array>	m_texCoord0;
	osg::ref_ptr<osg::Array>	m_texCoord1;
};