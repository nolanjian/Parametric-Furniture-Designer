#pragma once
#include <osg/Group>
#include <string>
#include <memory>
#include "../muparserx/mpParser.h"
#include "../muparserx/mpDefines.h"
#include "../muparserx/mpTypes.h"

#include <fx/gltf.h>


class BaseObject : public osg::Group
{
public:
	BaseObject();
	virtual ~BaseObject();

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
	static osg::ref_ptr<BaseObject> GLTF2OSG(std::shared_ptr<fx::gltf::Document> gltfObject);
	static std::shared_ptr<fx::gltf::Document> OSG2GLTF(osg::ref_ptr<BaseObject> pObj);

	static osg::ref_ptr<BaseObject> GLTFNode2BaseObject(std::shared_ptr<fx::gltf::Document> gltfObject, int32_t nodeID);

protected:
	bool LoadRotation(const fx::gltf::Node& node);
	bool LoadScale(const fx::gltf::Node& node);
	bool LoadTranslation(const fx::gltf::Node& node);
	bool LoadMatrix(const fx::gltf::Node& node);
	bool LoadParams(const fx::gltf::Node& node);
	bool LoadMesh(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Node& node);
	osg::ref_ptr<osg::Drawable> LoadPrimitive(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive);

	osg::Vec4	vRotation;
	osg::Vec3	vScale;
	osg::Vec3	vTranslation;
	osg::Matrixd	mat;

	std::map<std::wstring, std::wstring>	m_formulas;
	std::map<std::wstring, std::wstring>	m_formulasResult4Cal;
	std::map<std::wstring, std::wstring>	m_parentFormulasResult;

	mup::var_maptype	m_formulasResult;

private:
	unsigned long long int	m_ID = 0;
	bool m_bDirty = true;
	osg::ref_ptr<BaseObject>	m_parent;
	std::shared_ptr<mup::ParserX>	m_parser;
};