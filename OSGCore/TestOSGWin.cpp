#include "pch.h"
#include "TestOSGWin.h"
#include "BaseObject.h"


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
	const std::string vertexSource =
		"#version 330 \n"
		" \n"
		"uniform mat4 osg_ModelViewProjectionMatrix; \n"
		"uniform mat3 osg_NormalMatrix; \n"
		"uniform vec3 ecLightDir; \n"
		"uniform int useBaseColorFactor; \n"
		"uniform vec4 baseColorFactor; \n"
		" \n"
		"in vec4 osg_Vertex; \n"
		"in vec3 osg_Normal; \n"
		"out vec2 texcoord;\n"
		" \n"
		"void main() \n"
		"{ \n"
		"    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); \n"
		"    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); \n"
		"    //color = vec4( vec3( diffuse ), 1. ); \n"
		"    texcoord = gl_MultiTexCoord0.xy;	\n"
		" \n"
		"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
		"} \n";
	osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, vertexSource);

	const std::string fragmentSource =
		"#version 330 \n"
		" \n"
		"uniform sampler2D baseTexture; \n"
		"in vec2 texcoord;\n"
		"uniform int useBaseColorFactor; \n"
		"uniform vec4 baseColorFactor; \n"
		" \n"
		"void main() \n"
		"{ \n"
		"	 if (useBaseColorFactor == 1)"
		"	 {"
		"		gl_FragColor = baseColorFactor";
		"	 }"
		"	 else"
		"	 {"
		"		gl_FragColor = texture2D( baseTexture, texcoord); \n"
		"	 }"
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
