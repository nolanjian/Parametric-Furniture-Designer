#include "pch.h"
#include "TestOSGWin.h"
#include "BaseObject.h"

#include <fstream>
#include <sstream>


void TestOSGWin::Run(const std::string& path)
{
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(100, 100, 800, 600);

	auto scene = BaseObject::LoadSceneFromJsonFile(path);
	if (scene)
	{
		//osgUtil::Optimizer optimizer;
		//optimizer.optimize(scene.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS | osgUtil::Optimizer::TESSELLATE_GEOMETRY);
		configureShaders(scene->getOrCreateStateSet());
	}

	osg::Camera* cam = viewer.getCamera();
	cam->setProjectionMatrix(osg::Matrix::perspective(30., (double)800 / (double)600, 1., 100.));
	cam->setClearColor(osg::Vec4(94.0 / 255.0, 112 / 255.0, 129 / 255.0, 1.0));

	osgViewer::Viewer::Windows windows;
	viewer.getWindows(windows);
	for (osgViewer::Viewer::Windows::iterator itr = windows.begin();
		itr != windows.end();
		++itr)
	{
		osg::State* s = (*itr)->getState();
		s->setUseModelViewAndProjectionUniforms(true);
		s->setUseVertexAttributeAliasing(true);
	}

	//viewer.addEventHandler(new KeyboardEventHandler(tessInnerU, tessOuterU));
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.setSceneData(scene.get());
	viewer.run();
}

void TestOSGWin::configureShaders(osg::StateSet* stateSet)
{
	std::cout << std::filesystem::current_path().string();

	std::ifstream	ifVertex("D:\\Library\\Parametric-Furniture-Designer\\OSGCore\\VERTEX.glsl", std::ios::in);
	if (!ifVertex.is_open())
	{
		return;
	}
	std::string vertexSource;
	std::stringstream	ss1;
	ss1 << ifVertex.rdbuf();
	vertexSource = ss1.str();

	osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, vertexSource);
	
	std::ifstream	ifFRAGMENT("D:\\Library\\Parametric-Furniture-Designer\\OSGCore\\FRAGMENT.glsl", std::ios::in);
	if (!ifFRAGMENT.is_open())
	{
		return;
	}
	std::string fragmentSource;
	std::stringstream	ss2;
	ss2 << ifFRAGMENT.rdbuf();
	fragmentSource = ss2.str();
	
	ifFRAGMENT >> fragmentSource;
	osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, fragmentSource);

	osg::Program* program = new osg::Program;
	program->addShader(vShader);
	program->addShader(fShader);
	stateSet->setAttribute(program);

	osg::Vec3f lightDir(0., 0.5, 1.);
	lightDir.normalize();
	stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));
}
