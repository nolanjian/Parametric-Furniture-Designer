#include "BasePanel.h"

namespace PFDComponent
{
	BasePanel::BasePanel()
		:Parametric()
	{

	}

	BasePanel::~BasePanel()
	{
	}

	bool BasePanel::InitFromParams()
	{
		if (m_formulasResult.find(_T("W")) != m_formulasResult.end())
		{
			dWidth = m_formulasResult[_T("W")]->AsIValue()->GetFloat();
		}
		if (m_formulasResult.find(_T("D")) != m_formulasResult.end())
		{
			dDepth = m_formulasResult[_T("D")]->AsIValue()->GetFloat();
		}
		if (m_formulasResult.find(_T("H")) != m_formulasResult.end())
		{
			dHeight = m_formulasResult[_T("H")]->AsIValue()->GetFloat();
		}

		return true;
	}

	void BasePanel::SetDepth(double dVal)
	{
		dDepth = dVal;
	}

	double BasePanel::GetDepth()
	{
		return dDepth;
	}

	void BasePanel::SetHeight(double dVal)
	{
	}

	double BasePanel::GetHeight()
	{
		return 0.0;
	}

	void BasePanel::SetWidth(double dVal)
	{
	}

	double BasePanel::GetWidth()
	{
		return 0.0;
	}

	bool BasePanel::Update()
	{


		return false;
	}

	/*bool BasePanel::UpdatePrimitive()
	{
		if (m_geo)
		{
			removeChild(m_geo);
			m_geo = new osg::Geode();
		}

		osg::ref_ptr<osg::Array> pVertexIndexArray = GenerateVertexIndexArray();
		if (!pVertexIndexArray)
		{
			return false;
		}


		osg::ref_ptr<osg::DrawElementsUInt>	pDrawElements = new osg::DrawElementsUInt(
			osg::PrimitiveSet::TRIANGLES,
			pVertexIndexArray->getNumElements(),
			(const GLuint*)(pVertexIndexArray->getDataPointer()));

		osg::ref_ptr<osg::Geometry>	pGeometry = new osg::Geometry();
		pGeometry->addPrimitiveSet(pDrawElements);
		pGeometry->setVertexArray(GenerateVertexDataArray());
		pGeometry->setNormalArray(GenerateNormalDataArray(), osg::Array::Binding::BIND_PER_VERTEX);

		assert(m_geo);
		m_geo->addDrawable(pGeometry);

		ParametricComponent::UpdatePrimitive();

		return true;
	}*/

	osg::ref_ptr<osg::Array> BasePanel::GenerateVertexDataArray()
	{
		osg::ref_ptr<osg::Vec3Array> pArr = new osg::Vec3Array();

		pArr->push_back(osg::Vec3(0, 0, 0));
		pArr->push_back(osg::Vec3(dWidth, 0, 0));
		pArr->push_back(osg::Vec3(dWidth, -dDepth, 0));
		pArr->push_back(osg::Vec3(0, -dDepth, 0));
		pArr->push_back(osg::Vec3(0, 0, dHeight));
		pArr->push_back(osg::Vec3(dWidth, 0, dHeight));
		pArr->push_back(osg::Vec3(dWidth, -dDepth, dHeight));
		pArr->push_back(osg::Vec3(0, -dDepth, dHeight));

		return pArr;
	}

	osg::ref_ptr<osg::Array> BasePanel::GenerateVertexIndexArray()
	{
		osg::ref_ptr<osg::IntArray> pArr = new osg::IntArray();

#pragma region Face0
		pArr->push_back(0);
		pArr->push_back(1);
		pArr->push_back(2);

		pArr->push_back(0);
		pArr->push_back(2);
		pArr->push_back(3);
#pragma endregion Face0
#pragma region Face1
		pArr->push_back(3);
		pArr->push_back(2);
		pArr->push_back(6);

		pArr->push_back(3);
		pArr->push_back(6);
		pArr->push_back(7);
#pragma endregion Face1
#pragma region Face2
		pArr->push_back(1);
		pArr->push_back(5);
		pArr->push_back(6);

		pArr->push_back(1);
		pArr->push_back(6);
		pArr->push_back(2);
#pragma endregion Face2
#pragma region Face3
		pArr->push_back(0);
		pArr->push_back(4);
		pArr->push_back(5);

		pArr->push_back(0);
		pArr->push_back(5);
		pArr->push_back(1);
#pragma endregion Face3
#pragma region Face4
		pArr->push_back(0);
		pArr->push_back(3);
		pArr->push_back(7);

		pArr->push_back(0);
		pArr->push_back(7);
		pArr->push_back(4);
#pragma endregion Face4
#pragma region Face5
		pArr->push_back(4);
		pArr->push_back(7);
		pArr->push_back(6);

		pArr->push_back(4);
		pArr->push_back(6);
		pArr->push_back(5);
#pragma endregion Face5

		return pArr;
	}

	osg::ref_ptr<osg::Array> BasePanel::GenerateNormalDataArray()
	{
		osg::ref_ptr<osg::Vec3Array> pArr = new osg::Vec3Array();

#pragma region Face0
		pArr->push_back(osg::Vec3(0, 0, -1));
		pArr->push_back(osg::Vec3(0, 0, -1));
		pArr->push_back(osg::Vec3(0, 0, -1));

		pArr->push_back(osg::Vec3(0, 0, -1));
		pArr->push_back(osg::Vec3(0, 0, -1));
		pArr->push_back(osg::Vec3(0, 0, -1));
#pragma endregion Face0
#pragma region Face1
		pArr->push_back(osg::Vec3(0, -1, 0));
		pArr->push_back(osg::Vec3(0, -1, 0));
		pArr->push_back(osg::Vec3(0, -1, 0));

		pArr->push_back(osg::Vec3(0, -1, 0));
		pArr->push_back(osg::Vec3(0, -1, 0));
		pArr->push_back(osg::Vec3(0, -1, 0));
#pragma endregion Face1
#pragma region Face2
		pArr->push_back(osg::Vec3(1, 0, 0));
		pArr->push_back(osg::Vec3(1, 0, 0));
		pArr->push_back(osg::Vec3(1, 0, 0));

		pArr->push_back(osg::Vec3(1, 0, 0));
		pArr->push_back(osg::Vec3(1, 0, 0));
		pArr->push_back(osg::Vec3(1, 0, 0));
#pragma endregion Face2
#pragma region Face3
		pArr->push_back(osg::Vec3(0, 1, 0));
		pArr->push_back(osg::Vec3(0, 1, 0));
		pArr->push_back(osg::Vec3(0, 1, 0));

		pArr->push_back(osg::Vec3(0, 1, 0));
		pArr->push_back(osg::Vec3(0, 1, 0));
		pArr->push_back(osg::Vec3(0, 1, 0));
#pragma endregion Face3
#pragma region Face4
		pArr->push_back(osg::Vec3(-1, 0, 0));
		pArr->push_back(osg::Vec3(-1, 0, 0));
		pArr->push_back(osg::Vec3(-1, 0, 0));

		pArr->push_back(osg::Vec3(-1, 0, 0));
		pArr->push_back(osg::Vec3(-1, 0, 0));
		pArr->push_back(osg::Vec3(-1, 0, 0));
#pragma endregion Face4
#pragma region Face5
		pArr->push_back(osg::Vec3(0, 0, 1));
		pArr->push_back(osg::Vec3(0, 0, 1));
		pArr->push_back(osg::Vec3(0, 0, 1));

		pArr->push_back(osg::Vec3(0, 0, 1));
		pArr->push_back(osg::Vec3(0, 0, 1));
		pArr->push_back(osg::Vec3(0, 0, 1));
#pragma endregion Face5

		return pArr;
	}
}


