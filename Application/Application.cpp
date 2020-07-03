#include "Application.h"
#include <Scene/OpenGLWidget.h>

Application::Application(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	QMenu* menuFile = menuBar()->addMenu("&File");
	QAction* action = menuFile->addAction("&Open");

	PFD::Scene::OpenGLWidget* oglWidget = new PFD::Scene::OpenGLWidget();
	setCentralWidget(oglWidget);

	connect(action, SIGNAL(triggered()), oglWidget, SLOT(OpenFile()));

	//connect(action, SIGNAL(triggered()), this, SLOT(testSlots()));
}

void Application::testSlots()
{
	int nn = 0;
	nn = 99;
}