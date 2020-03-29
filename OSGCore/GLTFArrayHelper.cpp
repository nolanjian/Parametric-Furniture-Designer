#include "pch.h"
#include "GLTFArrayHelper.h"

GLTFArrayHelper& GLTFArrayHelper::GetInstance()
{
	static GLTFArrayHelper s_GLTFArrayHelper;
	return s_GLTFArrayHelper;
}

void GLTFArrayHelper::Load()
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
			mapArray[ii] = pArr;
			continue;
		}
		osg::ref_ptr<osg::RefMatrix> mat = GetOSGMatrix(accessors);
		if (mat)
		{
			mapMatrix[ii] = mat;
			continue;
		}

		LOG(WARNING) << "accessors " + std::to_string(ii) + "unhandled";
	}
}

osg::ref_ptr<osg::Array> GLTFArrayHelper::GetArrayByAccessorIndex(size_t index)
{
	return mapArray[index];
}

osg::ref_ptr<osg::RefMatrix> GLTFArrayHelper::GetOSGMatrix(const fx::gltf::Accessor& accessor)
{
	if (accessor.type != fx::gltf::Accessor::Type::Mat4 && accessor.componentType == fx::gltf::Accessor::ComponentType::Float)
	{
		LOG(WARNING) << "GetOSGMatrix only support Mat4 and Float";
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

osg::ref_ptr<osg::Array> GLTFArrayHelper::GetOSGArray(const fx::gltf::Accessor& accessor)
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
		LOG(WARNING) << "GetVecArray only support Vec2,Vec3,Vec4";
		return nullptr;
	}
}

template<class ArrayType>
inline osg::ref_ptr<osg::Array> GLTFArrayHelper::GetOSGVecArrayImp(const fx::gltf::Accessor& accessor)
{
	osg::ref_ptr<ArrayType> pRet = new ArrayType();

	fx::gltf::BufferView const& bufferView = m_gltfObject->bufferViews[accessor.bufferView];
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
inline osg::ref_ptr<osg::Array> GLTFArrayHelper::GetOSGArrayImp(const fx::gltf::Accessor& accessor)
{
	osg::ref_ptr<ArrayType>	pRet = new ArrayType();

	fx::gltf::BufferView const& bufferView = m_gltfObject->bufferViews[accessor.bufferView];
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