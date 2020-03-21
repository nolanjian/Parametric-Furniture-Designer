#include "pch.h"
#include "TestOSGWin.h"
#include "BaseObject.h"


void TestOSGWin::Run(const std::string& path)
{
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(100, 100, 800, 600);

	auto scene = BaseObject::LoadSceneFromJsonFile(path);

	osgUtil::Optimizer optimizer;
	optimizer.optimize(scene.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS | osgUtil::Optimizer::TESSELLATE_GEOMETRY);

	configureShaders(scene->getOrCreateStateSet());

	osg::Camera* cam = viewer.getCamera();
	cam->setProjectionMatrix(osg::Matrix::perspective(30., (double)800 / (double)600, 1., 100.));

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
	const std::string vertexSource =
		"#version 330 \n"
		" \n"
		"uniform mat4 osg_ModelViewProjectionMatrix; \n"
		"uniform mat3 osg_NormalMatrix; \n"
		"uniform vec3 ecLightDir; \n"
		" \n"
		"in vec4 osg_Vertex; \n"
		"in vec3 osg_Normal; \n"
		"out vec4 color; \n"
		" \n"
		"void main() \n"
		"{ \n"
		"    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); \n"
		"    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); \n"
		"    color = vec4( vec3( diffuse ), 1. ); \n"
		" \n"
		"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
		"} \n";
	osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, vertexSource);

	const std::string fragmentSource =
		"#version 330 \n"
		" \n"
		"in vec4 color; \n"
		"out vec4 fragData; \n"
		" \n"
		"void main() \n"
		"{ \n"
		"    fragData = color; \n"
		"} \n";
	osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, fragmentSource);

	osg::Program* program = new osg::Program;
	program->addShader(vShader);
	program->addShader(fShader);
	stateSet->setAttribute(program);

	osg::Vec3f lightDir(0., 0.5, 1.);
	lightDir.normalize();
	stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));
}
