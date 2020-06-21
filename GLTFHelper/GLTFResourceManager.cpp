#include "GLTFResourceManager.h"
#include <Commom/ShadingPreDefine.h>
#include <Utils/Utils.h>

namespace PFD
{
	namespace GLTF
	{

		GLTFResourceManager::GLTFResourceManager(std::shared_ptr<fx::gltf::Document> gltfObject)
			:m_gltfObject(gltfObject)
		{
			m_strGLTFPath = m_gltfObject->extensionsAndExtras["LocalPath"];
			LoadArray();
			LoadTexture();
			LoadMesh();
		}

		osg::ref_ptr<osg::Array> GLTFResourceManager::GetArrayByAccessorIndex(size_t index)
		{
			if (m_mapArray.find(index) != m_mapArray.end())
			{
				return m_mapArray[index];
			}
			return nullptr;
		}

		osg::ref_ptr<osg::RefMatrix> GLTFResourceManager::GetOSGMatrix(const fx::gltf::Accessor& accessor)
		{
			if (accessor.type != fx::gltf::Accessor::Type::Mat4 && accessor.componentType == fx::gltf::Accessor::ComponentType::Float)
			{
				logger->warn("GetOSGMatrix only support Mat4 and Float");
				return nullptr;
			}

			fx::gltf::BufferView const& bufferView = m_gltfObject->bufferViews[accessor.bufferView];
			fx::gltf::Buffer const& buffer = m_gltfObject->buffers[bufferView.buffer];
			auto bufOffset = static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset;
			const uint8_t* pData = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset];
			if (!pData)
			{
				return nullptr;
			}

			osg::ref_ptr<osg::RefMatrix> mat = new osg::RefMatrix();
			mat->set(static_cast<float*>((void*)pData));
			return mat;
		}

		osg::ref_ptr<osg::Array> GLTFResourceManager::GetOSGArray(const fx::gltf::Accessor& accessor)
		{
			switch (accessor.type)
			{
			case fx::gltf::Accessor::Type::Scalar:
			{
				switch (accessor.componentType)
				{
				case fx::gltf::Accessor::ComponentType::Byte:
					return GetOSGArrayImp<osg::ByteArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedByte:
					return GetOSGArrayImp<osg::UByteArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Short:
					return GetOSGArrayImp<osg::ShortArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedShort:
					return GetOSGArrayImp<osg::UShortArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedInt:
					return GetOSGArrayImp<osg::UIntArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Float:
					return GetOSGArrayImp<osg::FloatArray>(accessor);
				default:
					return nullptr;
				}
				break;
			}
			case fx::gltf::Accessor::Type::Vec2:
			{
				switch (accessor.componentType)
				{
				case fx::gltf::Accessor::ComponentType::Byte:
					return GetOSGVecArrayImp<osg::Vec2bArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedByte:
					return GetOSGVecArrayImp<osg::Vec2ubArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Short:
					return GetOSGVecArrayImp<osg::Vec2sArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedShort:
					return GetOSGVecArrayImp<osg::Vec2usArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedInt:
					return GetOSGVecArrayImp<osg::Vec2uiArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Float:
					return GetOSGVecArrayImp<osg::Vec2Array>(accessor);
				default:
					return nullptr;
				}
				break;
			}
			case fx::gltf::Accessor::Type::Vec3:
			{
				switch (accessor.componentType)
				{
				case fx::gltf::Accessor::ComponentType::Byte:
					return GetOSGVecArrayImp<osg::Vec3bArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedByte:
					return GetOSGVecArrayImp<osg::Vec3ubArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Short:
					return GetOSGVecArrayImp<osg::Vec3sArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedShort:
					return GetOSGVecArrayImp<osg::Vec3usArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedInt:
					return GetOSGVecArrayImp<osg::Vec3uiArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Float:
					return GetOSGVecArrayImp<osg::Vec3Array>(accessor);
				default:
					return nullptr;
				}
				break;
			}
			case fx::gltf::Accessor::Type::Vec4:
			{
				switch (accessor.componentType)
				{
				case fx::gltf::Accessor::ComponentType::Byte:
					return GetOSGVecArrayImp<osg::Vec4bArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedByte:
					return GetOSGVecArrayImp<osg::Vec4ubArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Short:
					return GetOSGVecArrayImp<osg::Vec4sArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedShort:
					return GetOSGVecArrayImp<osg::Vec4usArray>(accessor);
				case fx::gltf::Accessor::ComponentType::UnsignedInt:
					return GetOSGVecArrayImp<osg::Vec4uiArray>(accessor);
				case fx::gltf::Accessor::ComponentType::Float:
					return GetOSGVecArrayImp<osg::Vec4Array>(accessor);
				default:
					return nullptr;
				}
				break;
			}
			default:
				logger->warn("GetVecArray only support Vec2,Vec3,Vec4");
				return nullptr;
			}
		}

		template<class ArrayType>
		inline osg::ref_ptr<osg::Array> GLTFResourceManager::GetOSGVecArrayImp(const fx::gltf::Accessor& accessor)
		{
			osg::ref_ptr<ArrayType> pRet = new ArrayType();

			int32_t bvIndex = accessor.bufferView == -1 ? 0 : accessor.bufferView;

			fx::gltf::BufferView const& bufferView = m_gltfObject->bufferViews[bvIndex];
			fx::gltf::Buffer const& buffer = m_gltfObject->buffers[bufferView.buffer];
			auto bufOffset = static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset;
			const uint8_t* pData = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset];
			if (!pData)
			{
				return nullptr;
			}

			size_t itemSize = ArrayType::ElementDataType::num_components;
			size_t elementSize = sizeof(ArrayType::ElementDataType::value_type);

			size_t totalSize = itemSize * elementSize * accessor.count;
			uint8_t* pBuf = (uint8_t*)malloc(totalSize * sizeof(uint8_t));
			if (!pBuf)
			{
				return nullptr;
			}
			memcpy(pBuf, pData, totalSize * sizeof(uint8_t));

			for (size_t ii = 0; ii < accessor.count; ++ii)
			{
				const uint8_t* pCurItem = pBuf + itemSize * elementSize * ii;
				if (!pCurItem)
				{
					continue;
				}

				typename ArrayType::ElementDataType	item;
				for (size_t jj = 0; jj < itemSize; ++jj)
				{
					const uint8_t* pCurElement = pCurItem + elementSize * jj;
					if (!pCurElement)
					{
						continue;
					}
					item[jj] = *((typename ArrayType::ElementDataType::value_type*)(pCurElement));
				}
				pRet->push_back(item);
			}

			free(pBuf);

			return pRet;
		}

		template<class ArrayType>
		inline osg::ref_ptr<osg::Array> GLTFResourceManager::GetOSGArrayImp(const fx::gltf::Accessor& accessor)
		{
			osg::ref_ptr<ArrayType>	pRet = new ArrayType();

			int32_t bvIndex = accessor.bufferView == -1 ? 0 : accessor.bufferView;

			fx::gltf::BufferView const& bufferView = m_gltfObject->bufferViews[bvIndex];
			fx::gltf::Buffer const& buffer = m_gltfObject->buffers[bufferView.buffer];
			auto bufOffset = static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset;
			const uint8_t* pData = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset];
			if (!pData)
			{
				return nullptr;
			}

			size_t itemSize = sizeof(typename ArrayType::ElementDataType);
			size_t totalSize = itemSize * accessor.count;

			uint8_t* pBuf = (uint8_t*)malloc(totalSize * sizeof(uint8_t));
			if (!pBuf)
			{
				return nullptr;
			}

			memcpy(pBuf, pData, totalSize * sizeof(uint8_t));

			for (size_t ii = 0; ii < accessor.count; ++ii)
			{
				const uint8_t* pCurItem = pBuf + itemSize * ii;
				if (!pCurItem)
				{
					continue;
				}

				typename ArrayType::ElementDataType	item;
				item = *((typename ArrayType::ElementDataType*)(pCurItem));
				pRet->push_back(item);
			}

			free(pBuf);

			return pRet;
		}

		osg::ref_ptr<osg::Texture> GLTFResourceManager::GetTexture(size_t index)
		{
			if (m_mapTexture.find(index) != m_mapTexture.end())
			{
				return m_mapTexture[index];
			}
			return nullptr;
		}

		osg::ref_ptr<osg::Geode> GLTFResourceManager::GetMesh(size_t index)
		{
			if (m_mapGeode.find(index) != m_mapGeode.end())
			{
				return m_mapGeode[index];
			}
			return nullptr;
		}

		void GLTFResourceManager::LoadArray()
		{
			if (m_gltfObject == nullptr)
			{
				return;
			}
			for (size_t ii = 0; ii < m_gltfObject->accessors.size(); ++ii)
			{
				const fx::gltf::Accessor& accessors = m_gltfObject->accessors[ii];
				osg::ref_ptr<osg::Array> pArr = GetOSGArray(accessors);
				if (pArr)
				{
					m_mapArray[ii] = pArr;
					continue;
				}
				osg::ref_ptr<osg::RefMatrix> mat = GetOSGMatrix(accessors);
				if (mat)
				{
					m_mapMatrix[ii] = mat;
					continue;
				}

				logger->warn("accessors {} unhandled", ii);
			}
		}

		void GLTFResourceManager::LoadTexture()
		{
			m_mapTexture.clear();
			if (m_gltfObject == nullptr)
			{
				return;
			}
			for (size_t ii = 0; ii < m_gltfObject->textures.size(); ++ii)
			{
				const fx::gltf::Texture& texture = m_gltfObject->textures[ii];
				osg::ref_ptr<osg::Texture2D> pTexture = new osg::Texture2D();
				pTexture->setName(texture.name);

				if (texture.sampler >= 0 && texture.sampler < m_gltfObject->samplers.size())
				{
					LoadSampler(pTexture, m_gltfObject->samplers[texture.sampler]);
				}
				if (texture.source >= 0 && texture.source < m_gltfObject->images.size())
				{
					LoadImage(pTexture, m_gltfObject->images[texture.source]);
				}
				m_mapTexture[ii] = pTexture;
			}
		}

		void GLTFResourceManager::LoadSampler(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Sampler& sampler)
		{
			if (pTexture == nullptr || sampler.empty())
			{
				return;
			}

			if (m_mapWrapMode.find(sampler.wrapS) != m_mapWrapMode.end())
			{
				pTexture->setWrap(osg::Texture::WRAP_S, m_mapWrapMode.at(sampler.wrapS));
			}
			if (m_mapWrapMode.find(sampler.wrapT) != m_mapWrapMode.end())
			{
				pTexture->setWrap(osg::Texture::WRAP_T, m_mapWrapMode.at(sampler.wrapT));
			}
			if (m_mapMagFilter.find(sampler.magFilter) != m_mapMagFilter.end())
			{
				pTexture->setFilter(osg::Texture::MAG_FILTER, m_mapMagFilter.at(sampler.magFilter));
			}
			if (m_mapMinFilter.find(sampler.minFilter) != m_mapMinFilter.end())
			{
				pTexture->setFilter(osg::Texture::MIN_FILTER, m_mapMinFilter.at(sampler.minFilter));
			}
		}

		void GLTFResourceManager::LoadImage(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Image& image)
		{
			if (pTexture == nullptr)
			{
				return;
			}

			try
			{
				osg::ref_ptr<osg::Image>	pImage;

				auto fnLoadEmbeddedResource = [&]() -> bool
				{
					try
					{
						if (!image.IsEmbeddedResource())
						{
							return false;
						}

						std::vector<uint8_t>	imgData;
						image.MaterializeData(imgData);
						if (imgData.empty())
						{
							return false;
						}

						pImage = PFD::Utils::LoadImageFromMemory(&imgData[0]);
						return pImage != nullptr;
					}
					catch (const std::exception& e)
					{
						logger->error("fnLoadEmbeddedResource:\n{}", e.what());
						return false;
					}
				};

				auto fnLoadExteralResource = [&]() -> bool
				{
					try
					{
						std::string filePath = fx::gltf::detail::CreateBufferUriPath(fx::gltf::detail::GetDocumentRootPath(m_strGLTFPath), image.uri);
						if (!std::filesystem::exists(filePath))
						{
							return false;
						}

						pImage = PFD::Utils::LoadImageFromPath(filePath);
						return pImage != nullptr;
					}
					catch (const std::exception& e)
					{
						logger->error("fnLoadExteralResource:\n{}", e.what());
						return false;
					}
				};

				auto fnLoadBufferViewResource = [&]() -> bool
				{
					try
					{
						if (image.bufferView >= 0)
						{
							const fx::gltf::BufferView bufferView = m_gltfObject->bufferViews[image.bufferView];
							fx::gltf::Buffer const& buffer = m_gltfObject->buffers[bufferView.buffer];
							auto bufOffset = static_cast<uint64_t>(bufferView.byteOffset);
							const uint8_t* pData = &buffer.data[static_cast<uint64_t>(bufferView.byteOffset)];
							if (!pData)
							{
								return false;
							}

							pImage = PFD::Utils::LoadImageFromMemory(const_cast<unsigned char*>(pData));
							return pImage != nullptr;
						}
						return false;
					}
					catch (const std::exception& e)
					{
						logger->error("fnLoadBufferViewResource:\n{0}", e.what());
						return false;
					}
				};

				if (fnLoadEmbeddedResource() || fnLoadExteralResource() || fnLoadBufferViewResource())
				{
					if (pImage->valid())
					{
						pTexture->setImage(pImage);
					}
				}
			}
			catch (const std::exception& e)
			{
				logger->error("LoadImage:\n{}", e.what());
				return;
			}
		}

		void GLTFResourceManager::LoadMesh()
		{
			m_mapGeode.clear();
			if (m_gltfObject == nullptr)
			{
				return;
			}

			for (size_t ii = 0; ii < m_gltfObject->meshes.size(); ++ii)
			{
				const fx::gltf::Mesh& mesh = m_gltfObject->meshes[ii];
				osg::ref_ptr<osg::Geode>	geo = new osg::Geode();
				geo->setName(mesh.name);
				geo->getOrCreateStateSet();
				m_mapGeode[ii] = geo;
				for (const fx::gltf::Primitive& primitive : mesh.primitives)
				{
					osg::ref_ptr<osg::Drawable> ptrDrawable = LoadDrawable(primitive);
					if (ptrDrawable)
					{
						geo->addDrawable(ptrDrawable);
					}
				}
			}
		}

		osg::ref_ptr<osg::Drawable> GLTFResourceManager::LoadDrawable(const fx::gltf::Primitive& primitive)
		{
			osg::ref_ptr<osg::PrimitiveSet> ptrPrimitiveSet = LoadPrimitiveSet(primitive);
			if (ptrPrimitiveSet == nullptr)
			{
				return nullptr;
			}

			osg::ref_ptr<osg::Geometry>	ptrRet = new osg::Geometry();
			ptrRet->getOrCreateStateSet();
			ptrRet->addPrimitiveSet(ptrPrimitiveSet);
			bool useTangent = false;
			bool useTexcoord0 = false;
			bool useTexcoord1 = false;

			for (const std::pair<std::string, uint32_t>& attribute : primitive.attributes)
			{
				if (attribute.first == "POSITION")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						ptrRet->setVertexArray(pArr);
						osg::DrawArrays* pDrawArrays = dynamic_cast<osg::DrawArrays*>(ptrPrimitiveSet.get());
						if (pDrawArrays)
						{
							pDrawArrays->setFirst(0);
							pDrawArrays->setCount(pArr->getNumElements());
						}
					}
				}
				else if (attribute.first == "NORMAL")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						ptrRet->setNormalArray(pArr, osg::Array::Binding::BIND_PER_VERTEX);
					}
				}
				else if (attribute.first == "TANGENT")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						ptrRet->setVertexAttribArray(TANGENT_INDEX, pArr, osg::Array::Binding::BIND_PER_VERTEX);
						useTangent = true;
					}
				}
				else if (attribute.first == "TEXCOORD_0")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						ptrRet->setTexCoordArray(0, pArr, osg::Array::Binding::BIND_PER_VERTEX);
						useTexcoord0 = true;
					}
				}
				else if (attribute.first == "TEXCOORD_1")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						ptrRet->setTexCoordArray(1, pArr, osg::Array::Binding::BIND_PER_VERTEX);
						useTexcoord1 = true;
					}
				}
				else if (attribute.first == "JOINTS_0")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{

					}
				}
				else if (attribute.first == "WEIGHTS_0")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						// TODO
					}
				}
				else if (attribute.first == "COLOR_0")
				{
					osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(attribute.second);
					if (pArr)
					{
						ptrRet->setColorArray(pArr, osg::Array::Binding::BIND_PER_VERTEX);
					}
				}
				else
				{
					assert(false);
				}
			}

			ptrRet->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_TANGENT, useTangent));
			ptrRet->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_TEXCOORD0, useTexcoord0));
			ptrRet->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_TEXCOORD1, useTexcoord1));

			if (m_gltfObject->materials.size() > primitive.material && primitive.material >= 0)
			{
				LoadMaterial(m_gltfObject->materials[primitive.material], ptrRet);
			}

			bool useVertexColor = false;
			if (ptrRet->getColorArray())
			{
				auto numColor = ptrRet->getColorArray()->getNumElements();
				if (numColor > 0)
				{
					useVertexColor = true;
				}
			}
			ptrRet->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_VERTEX_COLOR, useVertexColor));

			// TODO 
			// std::vector<Attributes> targets{};

			return ptrRet;
		}

		osg::ref_ptr<osg::PrimitiveSet> GLTFResourceManager::LoadPrimitiveSet(const fx::gltf::Primitive& primitive)
		{
			if (primitive.indices >= 0)
			{
				return LoadDrawElements(primitive);
			}
			else
			{
				if (m_mapPrimitives.find(primitive.mode) != m_mapPrimitives.end())
				{
					return new osg::DrawArrays(m_mapPrimitives.at(primitive.mode));
				}
				return new osg::DrawArrays(m_mapPrimitives.at(fx::gltf::Primitive::Mode::Triangles));
			}
		}

		osg::ref_ptr<osg::DrawElements> GLTFResourceManager::LoadDrawElements(const fx::gltf::Primitive& primitive)
		{
			assert(primitive.indices >= 0);
			if (primitive.indices < 0)
			{
				return nullptr;
			}

			osg::ref_ptr<osg::Array> pArr = GetArrayByAccessorIndex(primitive.indices);
			if (pArr == nullptr)
			{
				return nullptr;
			}

			if (m_mapPrimitives.find(primitive.mode) == m_mapPrimitives.end())
			{
				return nullptr;
			}

			const auto mode = m_mapPrimitives.at(primitive.mode);

			switch (pArr->getType())
			{
			case osg::Array::Type::ByteArrayType:
			case osg::Array::Type::UByteArrayType:
				return new osg::DrawElementsUByte(mode, pArr->getNumElements(), (const GLubyte*)(pArr->getDataPointer()));
			case osg::Array::Type::ShortArrayType:
			case osg::Array::Type::UShortArrayType:
				return new osg::DrawElementsUShort(mode, pArr->getNumElements(), (const GLushort*)(pArr->getDataPointer()));
			case osg::Array::Type::UIntArrayType:
			case osg::Array::Type::FloatArrayType:
				return new osg::DrawElementsUInt(mode, pArr->getNumElements(), (const GLuint*)(pArr->getDataPointer()));
			default:
				return nullptr;
			}
		}

		bool GLTFResourceManager::LoadMaterial(const fx::gltf::Material& material, osg::ref_ptr<osg::Geometry> pGeometry)
		{
			bool bRet1 = LoadPBRTexture(material.pbrMetallicRoughness, pGeometry);
			bool bRet2 = LoadNormalTexture(material.normalTexture, pGeometry);
			bool bRet3 = LoadOcclusionTexture(material.occlusionTexture, pGeometry);

			switch (material.alphaMode)
			{
			case  fx::gltf::Material::AlphaMode::Opaque:
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(ENABLE_ALPHA_CUTOFF, false));
				break;
			case  fx::gltf::Material::AlphaMode::Mask:
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(ENABLE_ALPHA_CUTOFF, true));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(ALPHA_CUTOFF, material.alphaCutoff));
				pGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
				break;
			case  fx::gltf::Material::AlphaMode::Blend:
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(ENABLE_ALPHA_CUTOFF, false));
				pGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
				break;
			default:
				break;
			}

			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(MATERIAL_DOUBLE_SIDED, material.doubleSided));

			bool useEmissiveTexture = false;
			if (!material.emissiveTexture.empty() && material.emissiveFactor != fx::gltf::defaults::NullVec3)
			{
				osg::ref_ptr<osg::Texture> pTexture = GetTexture(material.emissiveTexture.index);
				if (pTexture)
				{
					pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(EMISSIVE_TEXTURE_INDEX, pTexture);
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(EMISSIVE_TEXTURE, EMISSIVE_TEXTURE_INDEX));
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(EMISSIVE_TEXTURE_COORD, material.emissiveTexture.texCoord));
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(EMISSIVE_FACTOR,
						osg::Vec3f(material.emissiveFactor[0], material.emissiveFactor[1], material.emissiveFactor[2])));
					useEmissiveTexture = true;
				}
			}
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_EMISSIVE_TEXTURE, useEmissiveTexture));

			return bRet1 || bRet2 || bRet3;
		}

		bool GLTFResourceManager::LoadPBRTexture(const fx::gltf::Material::PBRMetallicRoughness& pbrMaterial, osg::ref_ptr<osg::Geometry> pGeometry)
		{
			if (pbrMaterial.empty())
			{
				return false;
			}

			bool useBaseColorFactor = false;
			if (pbrMaterial.baseColorFactor != fx::gltf::defaults::IdentityVec4)
			{
				osg::Vec4 objectColor(pbrMaterial.baseColorFactor[0], pbrMaterial.baseColorFactor[1],
					pbrMaterial.baseColorFactor[2], pbrMaterial.baseColorFactor[3]);
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(BASE_COLOR_FACTOR, objectColor));
				useBaseColorFactor = true;

			}
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_BASE_COLOR_FACTOR, useBaseColorFactor));

			bool useBaseColorTexture = false;
			if (!pbrMaterial.baseColorTexture.empty())
			{
				osg::ref_ptr<osg::Texture> pTexture = GetTexture(pbrMaterial.baseColorTexture.index);
				if (pTexture)
				{
					pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(BASE_COLOR_TEXTURE_INDEX, pTexture);
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(BASE_COLOR_TEXTURE, BASE_COLOR_TEXTURE_INDEX));
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(BASE_COLOR_TEXTURE_COORD, pbrMaterial.baseColorTexture.texCoord));
					useBaseColorTexture = true;
				}
			}
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_BASE_COLOR_TEXTURE, useBaseColorTexture));

			bool useMetallicRoughnessTexture = false;
			if (!pbrMaterial.metallicRoughnessTexture.empty())
			{
				osg::ref_ptr<osg::Texture> pTexture = GetTexture(pbrMaterial.metallicRoughnessTexture.index);
				if (pTexture)
				{
					pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(METALLIC_ROUGHNESS_TEXTURE_INDEX, pTexture);
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(METALLIC_ROUGHNESS_TEXTURE, METALLIC_ROUGHNESS_TEXTURE_INDEX));
					pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(METALLIC_ROUGHNESS_TEXTURE_COORD, pbrMaterial.metallicRoughnessTexture.texCoord));
					useMetallicRoughnessTexture = true;
				}
			}
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_METALLIC_ROUGHNESS_TEXTURE, useMetallicRoughnessTexture));

			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(ROUGHNESS_FACTOR, pbrMaterial.roughnessFactor));
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(METALLIC_FACTOR, pbrMaterial.metallicFactor));

			return useBaseColorFactor || useBaseColorTexture || useMetallicRoughnessTexture;
		}

		bool GLTFResourceManager::LoadNormalTexture(const fx::gltf::Material::NormalTexture& normalTexture, osg::ref_ptr<osg::Geometry> pGeometry)
		{
			if (normalTexture.empty())
			{
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_NORMAL_TEXTURE, false));
				return false;
			}

			osg::ref_ptr<osg::Texture> pTexture = GetTexture(normalTexture.index);
			if (pTexture)
			{
				pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(NORMAL_TEXTURE_INDEX, pTexture);
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(NORMAL_TEXTURE, NORMAL_TEXTURE_INDEX));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(NORMAL_TEXTURE_COORD, normalTexture.texCoord));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(NORMAL_TEXTURE_SCALE, normalTexture.scale));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_NORMAL_TEXTURE, true));
				return true;
			}
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_NORMAL_TEXTURE, false));

			return false;
		}

		bool GLTFResourceManager::LoadOcclusionTexture(const fx::gltf::Material::OcclusionTexture& occlusionTexture, osg::ref_ptr<osg::Geometry> pGeometry)
		{
			if (occlusionTexture.empty())
			{
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_OCCLUSION_TEXTURE, false));
				return false;
			}
			osg::ref_ptr<osg::Texture> pTexture = GetTexture(occlusionTexture.index);
			if (pTexture)
			{
				pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(OCCLUSION_TEXTURE_INDEX, pTexture);
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(OCCLUSION_TEXTURE, OCCLUSION_TEXTURE_INDEX));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(OCCLUSION_TEXTURE_COORD, occlusionTexture.texCoord));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(OCCLUSION_TEXTURE_STRENGTH, occlusionTexture.strength));
				pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_OCCLUSION_TEXTURE, true));
				return true;
			}
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_OCCLUSION_TEXTURE, false));

			return false;
		}
	}

}