#include "pch.h"
#include "Loader.h"
#include "Utils.h"

PFD_GLTF_Helper::Loader::Loader()
{
}

PFD_GLTF_Helper::Loader::~Loader()
{
}

osg::ref_ptr<osg::Group> PFD_GLTF_Helper::Loader::Load(const std::wstring& path)
{
	std::string strPath = PFDUtils::WStringToString(path);
	if (strPath.find(".glb") == std::string::npos)
	{
		doc = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(strPath));
	}
	else
	{
		doc = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromBinary(strPath));
	}

	return Load(doc);
}

osg::ref_ptr<osg::Group> PFD_GLTF_Helper::Loader::Load(const std::string& utf8Json)
{
	std::stringstream	ss;
	ss << utf8Json;
	doc = std::make_shared<fx::gltf::Document>(fx::gltf::LoadFromText(ss, fx::gltf::detail::GetDocumentRootPath(utf8Json)));
	doc->extensionsAndExtras["LocalPath"] = std::filesystem::current_path().string();

	return Load(doc);
}

osg::ref_ptr<osg::Group> PFD_GLTF_Helper::Loader::Load(std::shared_ptr<fx::gltf::Document> gltf)
{
	doc = gltf;
	// TODO
	return osg::ref_ptr<osg::Group>();
}
