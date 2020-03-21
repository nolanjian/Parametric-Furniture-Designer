#pragma once



#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "../muparserx/mpParser.h"
#include "../muparserx/mpDefines.h"
#include "../muparserx/mpTypes.h"

#include <fx/gltf.h>

struct RawBufferInfo 
{
	const fx::gltf::Accessor* accessor = nullptr;
	const uint8_t* data = nullptr;
	uint32_t dataStride = 0;
	uint32_t totalSize = 0;
	
	bool Valid() const noexcept { return data != nullptr; }
};

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
	static osg::ref_ptr<BaseObject> GLTF2OSG(std::shared_ptr<fx::gltf::Document> gltfObject);
	static std::shared_ptr<fx::gltf::Document> OSG2GLTF(osg::ref_ptr<BaseObject> pObj);

	static std::shared_ptr<RawBufferInfo> GetRawBufferInfo(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Accessor& accessor);

	static osg::ref_ptr<osg::Vec4dArray> GetVec4Array(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Accessor& accessor);
	static std::vector<double>	GetNumArray(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Accessor& accessor);
	
	static uint32_t GetElementSize(const fx::gltf::Accessor& accessor);

	void	LoadTSRFromMatrix();
	void	InitFromDocument(std::shared_ptr<fx::gltf::Document> gltfObject);
	void	InitFromNode(std::shared_ptr<fx::gltf::Document> gltfObject,const fx::gltf::Node& curNode);
	int		GetClassType(const fx::gltf::Node& node);

	void	RetsetMatrixFromTSR();

	std::string	m_name;

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
	osg::ref_ptr<osg::Drawable> ImportPrimitive(std::shared_ptr<fx::gltf::Document> gltfObject, const fx::gltf::Primitive& primitive);
	bool ExportPrimitive(osg::ref_ptr<osg::Drawable> ptrDrawable, std::shared_ptr<fx::gltf::Document> gltfObject, fx::gltf::Primitive& primitive);

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

	osg::ref_ptr<osg::Vec4dArray> m_vertex;
	osg::ref_ptr<osg::Vec4dArray> m_normal;
	std::vector<double>				m_indexl;
	osg::ref_ptr<osg::Vec4dArray> m_texCoord0;
	osg::ref_ptr<osg::Vec4dArray> m_texCoord1;
	osg::ref_ptr<osg::Vec4dArray> m_texCoord2;
};