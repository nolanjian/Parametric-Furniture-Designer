#include "pch.h"
#include "STLIncluding.h"
#include "TestOSGWin.h"
#include "BaseObject.h"
#include "PickEventHandler.h"
#include "ShadingPreDefine.h"


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

	viewer.addEventHandler(new PickEventHandler(scene));
	//viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.setSceneData(scene.get());
	viewer.run();
}

std::string LoadString(const std::string& path)
{
	std::ifstream	fs(path);
	std::stringstream	ss;
	ss << fs.rdbuf();
	std::string source = ss.str();
	//LOG(INFO) << source;
	return source;
}

void TestOSGWin::configureShaders(osg::StateSet* stateSet)
{
	osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, LoadString("D:\\library\\Parametric-Furniture-Designer\\OSGCore\\VERTEX.vert"));
	osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, LoadString("D:\\library\\Parametric-Furniture-Designer\\OSGCore\\FRAGMENT.frag"));

	osg::Program* program = new osg::Program;
	program->addShader(vShader);
	program->addShader(fShader);
	program->addBindAttribLocation(TANGENT, TANGENT_INDEX);
	stateSet->setAttribute(program);

	osg::Vec3f lightDir(0., 0.5, 1.);
	lightDir.normalize();
	stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));
}
