#pragma once
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "Utils.h"
#include "spdlog/spdlog.h"
#include "spdlog/formatter.h"
#include <fx/gltf.h>
#include "stb_image.h"

namespace PFDCore
{

	class IGLTFResourceManager
	{
	public:
		IGLTFResourceManager() {}
		IGLTFResourceManager(const IGLTFResourceManager&) = delete;
		IGLTFResourceManager(const IGLTFResourceManager&&) = delete;
		IGLTFResourceManager& operator= (const IGLTFResourceManager&) = delete;
		IGLTFResourceManager& operator= (const IGLTFResourceManager&&) = delete;
		~IGLTFResourceManager() {}

		virtual void SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject) final;
		virtual void Load() = 0;
	protected:
		std::string	m_strGLTFPath;
		std::shared_ptr<fx::gltf::Document> m_gltfObject;
		std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);
	private:

	};
}